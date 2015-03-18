#include <iostream>
#include <fstream>
#include <math.h>
#include "GBN.h"

using namespace std;

void GBN::print(int count)
{
    if(!count)
    {
        cout << endl;
        return;
    }
    Packet x= buffer.front();
    buffer.pop();
    cout << x.sequenceNumber << " ";
    buffer.push(x);
    print(--count);
}

void GBN::finishSending(DiscreteEvent *event) {
    
    if (event == NULL) {
        cout << "Packet dropped..." << endl;
        return;
    }
    DES.push(*event);
    
    DiscreteEvent nextEvent = DES.top();
    DES.pop();
    if (nextEvent.getType() == TIMEOUT) {
        cout << "Popped TIMEOUT at: " << nextEvent.getTime() << endl;
        currentTime = nextEvent.getTime();
        clearBuffer();
        clearDES();
        bufferCount = 0;
        packetSequenceNumber = 0;
        return;
    } else if (nextEvent.getType() == ACK) {
        cout << "Popped ACK: " << nextEvent.getSequenceNumber() << endl;
        Packet packet = buffer.front();
        if (!nextEvent.getErrorFlag() && packet.sequenceNumber != nextEvent.getSequenceNumber()) {
            cout << "Finishing packet: " << nextEvent.getSequenceNumber() << endl;
            cout << "Difference: " << (nextEvent.getSequenceNumber() - packet.sequenceNumber) << endl;
            if (!buffer.empty()) {
                buffer.pop();
            }
            bufferCount--;
            sequenceNumber = (sequenceNumber + 1) % (n + 1);
            nextExpectedAck = (nextExpectedAck + 1) % (n + 1);
            numberOfPacketsFinished++;
            currentTime = nextEvent.getTime();
        } else {
            if (!buffer.empty()) {
                buffer.pop();
            }
            //sequenceNumber = (sequenceNumber + 1) % (n + 1);
            nextExpectedAck = (nextExpectedAck + 1) % (n + 1);
        }
    }
    
}

DiscreteEvent* GBN::send(int totalPacketLength, double ber) {
    
    // Sender
    bool errorFlag = false;
    
    int senderToReceiverErrorState = checkPacketForError(totalPacketLength, ber);
    if (senderToReceiverErrorState == 2) {
        return NULL;
    } else  if (senderToReceiverErrorState == 1) {
        errorFlag = true;
    }
    
    
    
    // Receiver
    if (!errorFlag && sequenceNumber == nextExpectedFrame) {
        nextExpectedFrame = (nextExpectedFrame + 1) % (n + 1);
        //cout << "Next Expected: " << nextExpectedFrame << endl;
    }
    
    int receiverToSenderErrorState = checkPacketForError(headerLength, ber);
    if (receiverToSenderErrorState == 2) {
        return NULL;
    } else  if (receiverToSenderErrorState == 1) {
        errorFlag = true;
    }
    
    currentTime = (buffer.front()).time + 2 * propagationDelay + ((double)headerLength / (double)channelCapacity);
    static DiscreteEvent event;
    int newSN = ((buffer.front()).sequenceNumber + 1) % (n + 1);
    cout << "Adding ACK with sequence: " << newSN  << " time: " << currentTime << endl;
    event.setValues(ACK, currentTime, errorFlag, newSN);
    return &event;
}

void GBN::purgeTimeout() {
    std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> copyDES;
    while(!DES.empty()) {
        DiscreteEvent top = DES.top();
        if (top.getType() != TIMEOUT) {
            copyDES.push(top);
        } else {
            //cout << "Deleted Timeout: " << top.getTime() << endl;
        }
        DES.pop();
    }
    DES = copyDES;
}

void GBN::clearBuffer() {
    std::queue<Packet> newBuffer;
    buffer = newBuffer;
}

void GBN::clearDES() {
    std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> newDES;
    DES = newDES;
}

void GBN::simulate(string outputFile) {
    n = 4;
    // Length of frame header (H)
    headerLength = 432;
    // Length of packet (l)
    packetLength = 12000;
    // Channel Capacity (C)
    channelCapacity = 5000000;
    
    // Experiment Duration
    numberOfPackets = 8;
    
    int totalPacketLength = headerLength + packetLength;
    double transmissionDelay = (double)totalPacketLength / (double)channelCapacity;
    
    double ber_array[3] = {0.0001, 0.00001, 0.0001};
    
    // CSV File
    ofstream output_file(outputFile);
    //for (double j = 2.5; j <= 12.5; j += 2.5) {
    for (double j = 2.5; j <= 2.5; j += 2.5) {
        //for (propagationDelay = 0.005; propagationDelay <= 0.25; propagationDelay += 0.245) {
        for (propagationDelay = 0.005; propagationDelay <= 0.005; propagationDelay += 0.245) {
            for (int k = 0; k < 1; k ++) {
                bitErrorRate = ber_array[k];
                delta = j * propagationDelay;
                
                // The sequence number of the sending frame (SN)
                sequenceNumber = 0;
                // The sequence number of the next expected ACK
                nextExpectedAck = 1;
                // The sequence number of the next expected frame
                nextExpectedFrame = 0;
                
                // The current time (tc)
                currentTime = 0;
                
                numberOfPacketsFinished = 0;
                
                clearDES();
                clearBuffer();
                bufferCount = 0;
                packetSequenceNumber = 0;
                
                while(numberOfPacketsFinished < numberOfPackets) {
                    double previousTime = currentTime;
                    bool newT1 = false;
                    cout << endl << endl;
                    cout << "Updating buffer..." << endl;
                    while(bufferCount < n) {
                        Packet packet;
                        packet.sequenceNumber = packetSequenceNumber;
                        packet.time = previousTime + transmissionDelay;
                        cout << "Adding packet: " << packet.sequenceNumber << " With time: " << packet.time << endl;
                        previousTime = packet.time;
                        buffer.push(packet);
                        bufferCount++;
                        packetSequenceNumber = (packetSequenceNumber + 1) % (n + 1);
                        newT1 = true;
                    }
                    
                    if (newT1) {
                        purgeTimeout();
                        double T1 = (buffer.front()).time + delta;
                        // Add a new timeout
                        cout << "Pushing timout: " << T1 << endl;
                        insert_event(TIMEOUT, T1, false, sequenceNumber);
                    }
                    
                    DiscreteEvent *returnedEvent = send(totalPacketLength, bitErrorRate);
                    finishSending(returnedEvent);
                }
                double throughPut = (double)(numberOfPacketsFinished * packetLength) / currentTime;
                cout << "---------" << endl;
                cout << fixed << throughPut << endl;
                //output_file.precision(2);
                //output_file << fixed << throughPut << ",";
            }
        }
        output_file << endl;
    }
}

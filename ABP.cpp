#include <iostream>
#include <fstream>
#include <math.h>
#include "ABP.h"

using namespace std;

void ABP::insert_event(EventType type, double time, bool errorFlag, int sequenceNumber) {
    DiscreteEvent event;
    event.setValues(type, time, errorFlag, sequenceNumber);
    DES.push(event);
}

// Returns 2 if the errorCount is >= 5, returns 1 if there is at least 1 error but less than 5 errors, returns 0 if there are no errors at all.
int checkPacketForError(int length, double ber) {
    int errorCount = 0;
    bool hasError = false;
    for (int i = 0; i < length; i ++) {
        double randomNumber = ((double)rand() / RAND_MAX);
        if (randomNumber < ber) {
            errorCount++;
            hasError = true;
        }
        
        if (errorCount >= 5) {
            return 2;
        }
    }
    
    if (hasError) {
        return 1;
    } else {
        return 0;
    }
}

DiscreteEvent* ABP::send(int totalPacketLength, double ber) {
    bool errorFlag = false;
    
    int senderToReceiverErrorState = checkPacketForError(totalPacketLength, ber);
    if (senderToReceiverErrorState == 2) {
        return NULL;
    } else  if (senderToReceiverErrorState == 1) {
        errorFlag = true;
    }
    
    if (!errorFlag && sequenceNumber == nextExpectedFrame) {
        nextExpectedFrame = (nextExpectedFrame + 1) % 2;
    }
    
    int receiverToSenderErrorState = checkPacketForError(headerLength, ber);
    if (receiverToSenderErrorState == 2) {
        return NULL;
    } else  if (receiverToSenderErrorState == 1) {
        errorFlag = true;
    }
    
    currentTime += 2 * propagationDelay + ((double)headerLength / (double)channelCapacity);
    static DiscreteEvent event;
    event.setValues(ACK, currentTime, errorFlag, nextExpectedFrame);
    return &event;
}

void ABP::finishSending(DiscreteEvent *event) {
    DiscreteEvent newTimeEvent;
    if (event == NULL) {
        newTimeEvent = DES.top();
    } else {
        DiscreteEvent ackEvent = *event;
        DES.push(ackEvent);
        newTimeEvent = DES.top();
        if (!ackEvent.getErrorFlag() && ackEvent.getSequenceNumber() == nextExpectedAck) {
            sequenceNumber = (sequenceNumber + 1) % 2;
            nextExpectedAck = (nextExpectedAck + 1) % 2;
            numberOfPacketsFinished++;
        }
        DES.pop();
    }
    currentTime = newTimeEvent.getTime();
    DES.pop();
}

void ABP::simulate() {
    // Length of frame header (H)
    headerLength = 432;
    // Length of packet (l)
    packetLength = 12000;
    // Channel Capacity (C)
    channelCapacity = 5000000;
    
    // Experiment Duration
    numberOfPackets = 5000;

    int totalPacketLength = headerLength + packetLength;
    double transmissionDelay = (double)totalPacketLength / (double)channelCapacity;
    
    double ber_array[3] = {0.0, 0.00001, 0.0001};
    
    // CSV File
    ofstream output_file("ABP.csv");
    for (double j = 2.5; j <= 12.5; j += 2.5) {
        for (propagationDelay = 0.005; propagationDelay <= 0.25; propagationDelay += 0.245) {
            for (int k = 0; k < 3; k ++) {
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
                
                // Start sending packets
                for (int i = 0; i < numberOfPackets; i++) {
                    double timeOutTime = currentTime + delta + transmissionDelay;
                    currentTime += transmissionDelay;
                    insert_event(TIMEOUT, timeOutTime, false, sequenceNumber);
                    DiscreteEvent *returnedEvent = send(totalPacketLength, bitErrorRate);
                    finishSending(returnedEvent);
                }
                double throughPut = (double)(numberOfPacketsFinished * packetLength) / currentTime;
                output_file << throughPut << ",";
            }
        }
        output_file << endl;
    }
}

int main() {
    // Seeding random number generator
    srand (time(NULL));
    ABP abpSimulator;
    abpSimulator.simulate();
    return 0;
}

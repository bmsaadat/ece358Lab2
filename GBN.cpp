#include <iostream>
#include <math.h>
#include "GBN.h"

using namespace std;

DiscreteEvent* GBN::send() {
    int totalPacketLength = headerLength + packetLength;
    double ber = bitErrorRate;
    // Sender
    bool errorFlag = false;
    
    int senderToReceiverErrorState = checkPacketForError(totalPacketLength, ber);
    if (senderToReceiverErrorState == 2) {
        return NULL;
    } else  if (senderToReceiverErrorState == 1) {
        errorFlag = true;
    }
    
    
    // Receiver
    if (!errorFlag && SN[counter] == nextExpectedFrame) {
        nextExpectedFrame = (nextExpectedFrame + 1) % (n + 1);
    }
    
    int receiverToSenderErrorState = checkPacketForError(headerLength, ber);
    if (receiverToSenderErrorState == 2) {
        return NULL;
    } else  if (receiverToSenderErrorState == 1) {
        errorFlag = true;
    }
    
    double newTime = T[counter] + 2 * propagationDelay + ((double)headerLength / (double)channelCapacity);
    static DiscreteEvent event;
    
    event.setValues(ACK, newTime, errorFlag, nextExpectedFrame);
    return &event;
}

void GBN::purgeTimeout() {
    std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> copyDES;
    while(!DES.empty()) {
        DiscreteEvent top = DES.top();
        if (top.getType() != TIMEOUT) {
            copyDES.push(top);
        }
        DES.pop();
    }
    DES = copyDES;
}

void GBN::clearDES() {
    std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> newDES;
    DES = newDES;
}

void GBN::senderFunction() {
    int totalPacketLength = headerLength + packetLength;
    double transmissionDelay = (double)totalPacketLength / (double)channelCapacity;

    while(counter < n) {
        if (numberOfPacketsFinished >= numberOfPackets) {
            break;
        }
        
        next_expected_ack[counter] = (SN[counter] + 1) % (n+1);

        currentTime += transmissionDelay;
        
        T[counter] = currentTime;
        if (counter == 0) {
            insert_event(TIMEOUT, T[counter] + delta, false, sequenceNumber);
        }
        DiscreteEvent *event = send();
        
        if (event != NULL) {
            DES.push(*event);
        }
        
        if (!DES.empty()) {
            DiscreteEvent firstEvent = DES.top();
            if (firstEvent.getTime() < T[counter] && firstEvent.getType() == TIMEOUT) {
                clearDES();
                counter = 0;
                
                nextExpectedFrame = SN[0];
                continue;
            }
            
            if (firstEvent.getTime() < T[counter] && firstEvent.getType() == ACK && !firstEvent.getErrorFlag()) {
                bool isNextExpected = false;
                for (int next_expected_index = 0; next_expected_index < next_expected_ack.size(); next_expected_index++) {
                    if (next_expected_ack[next_expected_index] == firstEvent.getSequenceNumber()) {
                        isNextExpected = true;
                    }
                }
                if (isNextExpected) {
                    P = SN[0];
                    shiftLeft((firstEvent.getSequenceNumber() - P + n + 1) % (n + 1));
                    purgeTimeout();
                    insert_event(TIMEOUT, T[0] + delta, false, sequenceNumber);
                } else {
                    DES.pop();
                }
            }
        }
        
        int tmpCount = counter + 1;
        if (tmpCount < n) {
            counter = tmpCount;
            SN[counter] = (SN[counter - 1] + 1) % (n + 1);
        } else {
            break;
        }
    }
}

void GBN::shiftLeft(int amount) {
    numberOfPacketsFinished += amount;
    
    counter = (counter - amount + n + 1) % (n + 1);
    
    // Shift
    if (amount > 0) {
        
        if (amount < n) {
            for (int i = amount; i < n; i ++) {
                SN[i-amount] = SN[i];
                T[i-amount] = T[i];
                next_expected_ack[i-amount] = next_expected_ack[i];
            }
        } else if (amount == n) {
            SN[0] = (SN[n-1] + 1) % (n + 1);
            next_expected_ack[0] = (next_expected_ack[n-1] + 1) % (n + 1);
            for (int i = 0; i < n - 1; i ++) {
                SN[i+1] = (SN[i] + 1) % (n + 1);
                next_expected_ack[i+1] = (next_expected_ack[i] + 1) % (n + 1);
            }
        }
        
    }
    // Fill
    for (int i = n - amount; i < n; i ++) {
        next_expected_ack[i] = (next_expected_ack[i - 1] + 1) % (n + 1);
    }
}

void GBN::simulate() {
    n = 4;
    // Length of frame header (H)
    headerLength = 432;
    // Length of packet (l)
    packetLength = 12000;
    // Channel Capacity (C)
    channelCapacity = 5000000;
    
    // Experiment Duration
    numberOfPackets = 10000;
    
    double ber_array[3] = {0, 0.00001, 0.0001};
    next_expected_ack.resize(n);
    SN.resize(n);
    T.resize(n);
    
    
    // CSV File
    for (double j = 2.5; j <= 12.5; j += 2.5) {
        for (propagationDelay = 0.005; propagationDelay <= 0.25; propagationDelay += 0.245) {
            for (int k = 0; k < 3; k ++) {
                bitErrorRate = ber_array[k];
                delta = j * propagationDelay;
                
                
                // The sequence number of the next expected frame
                nextExpectedFrame = 0;
                
                // The current time (tc)
                currentTime = 0;
                
                numberOfPacketsFinished = 0;
                
                clearDES();
                counter = 0;
                
                
                P = 0;
                for (int i = 0; i < n; i ++) {
                    SN[i] = 0;
                    next_expected_ack[i] = SN[i] + 1;
                }
                
                // Call sender function
                senderFunction();
                
                while(numberOfPacketsFinished < numberOfPackets) {
                    while (!DES.empty()) {
                        if (numberOfPacketsFinished >= numberOfPackets) {
                            break;
                        }
                        
                        DiscreteEvent nextEvent = DES.top();
                        DES.pop();
                        if (nextEvent.getType() == ACK && nextEvent.getErrorFlag() == true) {
                            continue;
                        }
                        
                        currentTime = nextEvent.getTime();
                        if (nextEvent.getType() == TIMEOUT) {
                            counter = 0;
                            nextExpectedFrame = SN[0];
                            clearDES();
                            senderFunction();
                        } else if (nextEvent.getType() == ACK && !nextEvent.getErrorFlag()) {
                            
                            bool isNextExpected = false;
                            for (int next_expected_index = 0; next_expected_index < next_expected_ack.size(); next_expected_index++) {
                                if (next_expected_ack[next_expected_index] == nextEvent.getSequenceNumber()) {
                                    isNextExpected = true;
                                }
                            }
                            
                            
                            if (isNextExpected) {
                                P = SN[0];
                                int amountShifted = (nextEvent.getSequenceNumber() - P + n + 1) % (n+1);
                                shiftLeft(amountShifted);
                                
                                purgeTimeout();
                                insert_event(TIMEOUT, T[0] + delta, false, sequenceNumber);
                                counter++;
                                SN[counter] = (SN[counter - 1] + 1) % (n + 1);
                                senderFunction();
                            }
                        }
                    }
                }
                
                clearDES();
                double throughPut = (double)(numberOfPackets * packetLength) / currentTime;                
                cout.precision(2);
                cout << fixed << throughPut << ",";
            }
        }
        cout << endl;
    }
}

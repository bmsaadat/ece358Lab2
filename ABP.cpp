#include <iostream>
#include "ABP.h"

using namespace std;

void ABP::insert_event(EventType type, double time, bool errorFlag, int sequenceNumber) {
    DiscreteEvent event;
    event.setValues(type, time, errorFlag, sequenceNumber);
    DES.push(event);
}

DiscreteEvent ABP::send() {
    DiscreteEvent event = DES.top();
    return event;
}

void ABP::simulate(double d) {
    // The sequence number of the sending frame (SN)
    sequenceNumber = 0;
    // The sequence number of the next expected ACK
    nextExpectedAck = 1;
    // The sequence number of the next expected frame
    nextExpectedFrame = 0;
    
    // Time-out value (delta)
    delta = d;
    // The current time (tc)
    currentTime = 0;
    // Length of frame header (H)
    headerLength = 1000;
    // Length of packet (l)
    packetLength = 1000;
    
    
    // Channel Capacity (C)
    channelCapacity = 1000;
    // Propagation delay (tau)
    propagationDelay = 0;
    // Bit Error Rate (BER)
    bitErrorRate = 0;
    
    // Experiment Duration
    numberOfPackets = 5000;

    int totalPacketLength = headerLength + packetLength;
    double transmissionDelay = (double)totalPacketLength / (double)channelCapacity;
    
    
    // Start sending packets
    for (int i = 0; i < numberOfPackets; i++) {
        insert_event(TIMEOUT, currentTime + delta + transmissionDelay, false, sequenceNumber);
        currentTime += transmissionDelay;
        send();
    }
    
    // Pseudo code
    // Create TIMEOUT event at time tc+L/C+delta
    // Call send()
        //
}

int main() {
    ABP abpSimulator;
    abpSimulator.simulate(10);
    return 0;
}

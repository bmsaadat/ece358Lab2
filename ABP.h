/*
 * ABP.h
 *
 *  Created on: Mar 14, 2015
 *      Author: behrozsaadat
 */

#include "Lab2_functions.h"
#include <queue>
#include <vector>
#include <functional>

class ABP {
protected:
    // The sequence number of the sending frame (SN)
	int sequenceNumber;
    // The sequence number of the next expected ACK
	int nextExpectedAck;
    // The sequence number of the next expected frame
    int nextExpectedFrame;
    
    // Time-out value (delta)
    double delta;
    // The current time (tc)
    double currentTime;
    // Length of frame header (H)
    int headerLength;
    // Length of packet (l)
    int packetLength;
    
    
    // Channel Capacity (C)
    int channelCapacity;
    // Propagation delay (tau)
    double propagationDelay;
    // Bit Error Rate (BER)
    double bitErrorRate;
    
    // Experiment Duration
    int numberOfPackets;

    
	// Priority queue of events, ordered by the time
	std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> DES;
	void insert_event(EventType type, double time, bool errorFlag, int sequenceNumber);
    DiscreteEvent send();
public:
	void simulate(double delta);
};


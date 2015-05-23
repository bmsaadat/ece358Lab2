/*
 * ABP.h
 *
 *  Created on: Mar 14, 2015
 *      Author: behrozsaadat
 */
#ifndef ABP_H_
#define ABP_H_
#include "Lab2_functions.h"
#include <queue>
#include <vector>
#include <functional>

using std::string;

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
    
    int numberOfPacketsFinished;

    
	// Priority queue of events, ordered by the time
	std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> DES;
    int checkPacketForError(int length, double ber);
	void insert_event(EventType type, double time, bool errorFlag, int sequenceNumber);
    DiscreteEvent* send(int packetLength, double bitErrorRate);
    virtual void finishSending(DiscreteEvent *event);
public:
	void simulate();
};
#endif
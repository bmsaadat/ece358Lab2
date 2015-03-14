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
	int sn = 0;
	int nextExpectedAck = 1;

	// Priority queue of events, ordered by the time
	std::priority_queue<DiscreteEvent, std::vector<DiscreteEvent>, LessThanByTime> DES;

	void insert_event(EventType type, double time, bool errorFlag, int sequenceNumber);
	void generate_arrival_packets();
	void generate_observer_packets();
public:
	void simulate(double row);
	void simulator();
};


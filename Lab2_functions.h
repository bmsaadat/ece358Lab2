/*
 * Lab2_functions.h
 *
 *  Created on: Mar 14, 2015
 *      Author: behrozsaadat
 */

#ifndef LAB2_FUNCTIONS_H_
#define LAB2_FUNCTIONS_H_

// Different types of events
enum EventType { TIMEOUT, ACK };

// DES Class
class DiscreteEvent {
	EventType eventType;
    double time;
    bool errorFlag;
    int sequenceNumber;
public:
	void setValues (EventType, double, bool, int);
	EventType getType() const;
	double getTime() const;
	bool getErrorFlag() const;
	int getSequenceNumber() const;
};

class Packet {
public:
    int sequenceNumber;
    double time;
};

struct LessThanByTime {
	bool operator()(const DiscreteEvent &lhs, const DiscreteEvent &rhs) const{
		return (lhs.getTime() > rhs.getTime());
	}
};

#endif /* LAB2_FUNCTIONS_H_ */

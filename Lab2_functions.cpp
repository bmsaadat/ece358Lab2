/*
 * Lab2_functions.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: behrozsaadat
 */
#include <iostream>
#include <math.h>
#include "Lab2_functions.h"

void DiscreteEvent::setValues (EventType e, double t, bool error, int sequence) {
	eventType = e;
	time = t;
	errorFlag = error;
	sequenceNumber = sequence;
}

EventType DiscreteEvent::getType () const {
	return eventType;
}

double DiscreteEvent::getTime () const {
	return time;
}

bool DiscreteEvent::getErrorFlag () const {
	return errorFlag;
}

int DiscreteEvent::getSequenceNumber () const {
	return sequenceNumber;
}






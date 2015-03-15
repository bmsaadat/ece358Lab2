#include <iostream>
#include <fstream>
#include <math.h>
#include "ABP_NAK.h"

using namespace std;

// The only difference is in this function, which we are overriding
void ABP_NAK::finishSending(DiscreteEvent *event) {
    DiscreteEvent timeOutEvent = DES.top();
    if (event == NULL) {
        currentTime = timeOutEvent.getTime();
    } else {
        DiscreteEvent ackEvent = *event;
        DES.push(ackEvent);
        if (!ackEvent.getErrorFlag() && ackEvent.getSequenceNumber() == nextExpectedAck) {
            // Set the current time to the earliest event in the DES
            currentTime = DES.top().getTime();
            sequenceNumber = (sequenceNumber + 1) % 2;
            nextExpectedAck = (nextExpectedAck + 1) % 2;
            numberOfPacketsFinished++;
        } else {
            // Set the current time to the timeout cause the packet has error and wasn't delivered properly
            currentTime = timeOutEvent.getTime();
        }
        DES.pop();
    }
    DES.pop();
}

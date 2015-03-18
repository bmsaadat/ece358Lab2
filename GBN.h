/*
 * GBN.h
 *
 *  Created on: Mar 17, 2015
 *      Author: behrozsaadat
 */
#ifndef GBN_H_
#define GBN_H_
#include "ABP.h"
using std::string;

// Subclassing from ABP class since these two are very, very similiar.
class GBN: public ABP {
protected:
    // Buffer size
    int n;
    std::queue<Packet> buffer;
    int bufferCount;
    int packetSequenceNumber;

    
    void finishSending();
    DiscreteEvent* send(int totalPacketLength, double ber);
    virtual void finishSending(DiscreteEvent *event);
    void purgeTimeout();
    void clearBuffer();
    void clearDES();
public:
    void simulate(string outputFile);
    void print(int num);
};
#endif
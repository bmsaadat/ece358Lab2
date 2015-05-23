/*
 * GBN.h
 *
 *  Created on: Mar 17, 2015
 *      Author: behrozsaadat
 */
#ifndef GBN_H_
#define GBN_H_
#include "ABP.h"
#include <vector>
using std::string;

// Subclassing from ABP class since these two are very, very similiar.
class GBN: public ABP {
protected:
    // Buffer size
    int n;
    int counter;
    int P;
    
    std::vector <int> next_expected_ack;
    std::vector <int> SN;
    std::vector <double> T;

    DiscreteEvent* send();
    void purgeTimeout();
    void clearDES();
    void senderFunction();
    void shiftLeft(int amount);
public:
    void simulate();
};
#endif
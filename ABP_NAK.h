/*
 * ABP_NAK.h
 *
 *  Created on: Mar 14, 2015
 *      Author: behrozsaadat
 */

#include "ABP.h"

// Subclassing from ABP class since these two are very, very similiar.
class ABP_NAK: public ABP {
protected:
    virtual void finishSending(DiscreteEvent *event);
};

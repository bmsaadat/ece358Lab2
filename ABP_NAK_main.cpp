#include <iostream>
#include <math.h>
#include "ABP_NAK.h"

using namespace std;

int main() {
    // Seeding random number generator
    srand (time(NULL));
    ABP_NAK simulator;
    simulator.simulate();
    return 0;
}
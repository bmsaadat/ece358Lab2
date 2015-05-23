#include <iostream>
#include <math.h>
#include "ABP.h"

using namespace std;

int main() {
    // Seeding random number generator
    srand (time(NULL));
    ABP simulator;
    simulator.simulate();
    return 0;
}
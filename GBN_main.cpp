#include <iostream>
#include <math.h>
#include "GBN.h"

using namespace std;

int main() {
    // Seeding random number generator
    srand (time(NULL));
    GBN simulator;
    simulator.simulate("GBN.csv");
    return 0;
}
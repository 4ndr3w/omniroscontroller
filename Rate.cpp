#include "Rate.h"
#include <Arduino.h>

Rate::Rate(int hz) {
    periodUs = (1.0/(double)hz) * 1000000;
    lastUs = 0;
}

void Rate::sleep() {
    int dt = micros() - lastUs;

    if ( dt < periodUs )
        delayMicroseconds(periodUs-dt);

    lastUs = micros();
}


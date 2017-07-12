#include "Watchdog.h"
#include <Arduino.h>

Watchdog::Watchdog(unsigned int maxAge) {
    lastFed = 0;
    this->maxAge = maxAge;
}

void Watchdog::feed() {
    lastFed = millis();
}

bool Watchdog::hungry() {
    return millis() - lastFed >= maxAge;
}
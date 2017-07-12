#ifndef RATE_H
#define RATE_H

class Rate {
public:
    Rate(int hz);
    bool needsRun();
    void sleep();
private:
    int periodUs;
    unsigned long lastUs;
};


#endif
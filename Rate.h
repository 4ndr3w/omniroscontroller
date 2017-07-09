#ifndef RATE_H
#define RATE_H

class Rate {
public:
    Rate(int hz);
    void sleep();
private:
    int periodUs;
    int lastUs;
};


#endif
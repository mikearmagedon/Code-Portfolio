#ifndef ELOCK_H
#define ELOCK_H

#include <wiringPi.h>

class ELock
{
private:
    int ELock_Pin;// Associated pin to the Electric Lock

public:
    ELock();//Constructor
    void open(void);//Opens the lock
    void close(void);//Closes the lock
};

#endif // ELOCK_H

#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <semaphore.h>

#ifndef DEBUG
#define DEBUG 0
#endif

using namespace std;

/* Descrpiptor of the semaphore sem_Rec*/
extern sem_t sem_Rec;

class Button
{
public:
    Button();//COnstructor

private:
    static void buttonInterrupt(void);// Handle of the button's pin interrupt
};

#endif // BUTTON_H

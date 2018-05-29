#include "button.h"

Button::Button()
{
    // sets up the wiringPi library
    if (wiringPiSetup() < 0) {
        cerr << "Unable to setup wiringPi" << endl;
    }

    // set GPIO26 as input and with pull-up
    pinMode(25, INPUT);
    pullUpDnControl(25, PUD_UP);

    // set GPIO26 to generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if ( wiringPiISR(25, INT_EDGE_FALLING, &Button::buttonInterrupt) < 0 ) {
        cerr << "Unable to setup ISR" << endl;
    }
}

void Button::buttonInterrupt(void) {
    static unsigned int integrator=0;
    if (++integrator == 2) {
#if DEBUG
        cout << "Button Pressed." << endl;
#endif
        delay(500);
        integrator=0;
        sem_post(&sem_Rec);
    }
}

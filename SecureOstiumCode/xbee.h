#ifndef XBEE_H
#define XBEE_H

#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <QtCore>

using namespace std;

#ifndef DEBUG
#define DEBUG 0
#endif

class XBee
{
    int fd;
    char msg_XBee[30];

public:
    XBee();//Constructor
    void open_usart(void);//Opens the XBee conection
    void close_usart(void);//Opens the XBee conection
    void send_toXBee_IS(void);// Sends request samples frame
    void send_toXBee_LED(int);//Sens LED frame
    int read_toXBee(int*,int*);// Reads the response samples frame
    int init_UART(const char *device, const int baud);
    int sequence_request(int *, int *, int);// Sartss the respective XBee's sequence
};

#endif // XBEE_H

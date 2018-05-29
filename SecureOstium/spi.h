#ifndef SPI_H
#define SPI_H

#include <pigpio.h>
#include <iostream>

using namespace std;

class SPI
{
private:

public:
    int m_pSpi;
    SPI();
    virtual void WriteRawRC(char);
    virtual unsigned char ReadRawRC(void);
    bool openSPI(int);
    bool closeSPI(void);
};

#endif // SPI_H

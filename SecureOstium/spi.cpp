#include "spi.h"

SPI::SPI()
{

}

bool SPI::openSPI(int channel)
{
    if (gpioInitialise()<0)
    {
        cerr << "Error in gpioInitialise()" << endl;
        return false;
    }

    if (channel == 0)
        m_pSpi = spiOpen(channel, 2000000, PI_SPI_FLAGS_AUX_SPI(1)|PI_SPI_FLAGS_RESVD(6));
    else if (channel == 1)
        m_pSpi = spiOpen(channel, 200000, PI_SPI_FLAGS_AUX_SPI(1)|PI_SPI_FLAGS_RESVD(5));
    else if (channel == 2)
        m_pSpi = spiOpen(channel, 200000, PI_SPI_FLAGS_AUX_SPI(1)|PI_SPI_FLAGS_RESVD(3));

    if (m_pSpi<0)
        return false;
    return true;
}

bool SPI::closeSPI(void)
{
    if (!spiClose(m_pSpi))
    {
        gpioTerminate();
        return true;
    }
    return false;
}

void SPI::WriteRawRC(char value)
{
    char *data = &value;
    spiWrite(m_pSpi, data, 1);
}

unsigned char SPI::ReadRawRC(void)
{
    char *idata = new char(0x00);
    char *odata = new char(0x00);
    spiXfer(m_pSpi, odata, idata, 1);
    return *idata;
}

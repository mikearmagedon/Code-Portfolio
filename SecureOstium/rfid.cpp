#include "rfid.h"

RFID::RFID()
{    
    wiringPiSetup();
    wiringPiSPISetup(1,1000000);
}

void RFID::WriteRawRC(unsigned char Address, unsigned char value)
{
    unsigned char ucAddr;
    unsigned char data[2] = {0, 0};
    ucAddr = ((Address<<1)&0x7E);	//address format：0XXXXXX0
    data[0] = ucAddr;
    data[1] = value;
    wiringPiSPIDataRW(1, data, 2);
}

unsigned char RFID::ReadRawRC(unsigned char Address)
{
    unsigned char ucAddr;
    unsigned char data = 0x00;
    ucAddr = ((Address<<1)&0x7E)|0x80;    	//address format：1XXXXXX0
    wiringPiSPIDataRW(1, &ucAddr, 1);
    wiringPiSPIDataRW(1, &data, 1);
    return data;
}

void RFID::init_MFRC522()
{
    uint8_t temp;

    WriteRawRC(MFRC522_REG_T_MODE, 0x8D);           // TModeReg defines settings for the internal timer
    WriteRawRC(MFRC522_REG_T_PRESCALER, 0x3E);      // TPrescalerReg
    WriteRawRC(MFRC522_REG_T_RELOAD_L	, 30);      // lowest 8-bit TReloadReg
    WriteRawRC(MFRC522_REG_T_RELOAD_H, 0x00);		// highest 8-bit TReloadReg
    WriteRawRC(MFRC522_REG_RF_CFG, 0x70);			// RFCfgReg configures the receiver gain
    WriteRawRC(MFRC522_REG_TX_AUTO	, 0x40);		// TxASKReg controls the setting of the transmission modulation
    WriteRawRC(MFRC522_REG_MODE, 0x3D);


    temp = ReadRawRC (MFRC522_REG_TX_CONTROL);		// TxControlReg controls the logical behavior of the antenna driver pins TX1 and TX2
    if (!(temp & 0x03)) {
        WriteRawRC(MFRC522_REG_TX_CONTROL, ReadRawRC(MFRC522_REG_TX_CONTROL) | 0x03);
    }
}


int RFID::Check_Card(char * buff_cmd, int len, uint8_t *buff_ID)
{
    uint8_t n=0;
    int i=0;
    int status=0;

    WriteRawRC(MFRC522_REG_COMM_IE_N, 0x77 | 0x80 ); //  enable and disable interrupt request control bits

    WriteRawRC(MFRC522_REG_COMM_IRQ, ReadRawRC(MFRC522_REG_COMM_IRQ) | (~0x80)); // ComIrqReg interrupt request bits
    WriteRawRC(MFRC522_REG_FIFO_LEVEL, ReadRawRC(MFRC522_REG_FIFO_LEVEL) | (0x80));	// FIFOLevelReg number of bytes stored in the FIFO buffer

    WriteRawRC(MFRC522_REG_COMMAND,0x00);

    //Writing data to the FIFO
    for (i = 0; i < len; i++) {
        WriteRawRC(MFRC522_REG_FIFO_DATA, buff_cmd[i]);  //FIFODataReg input and output of 64 byte FIFO buffer
    }

    //Execute the command
    WriteRawRC(MFRC522_REG_COMMAND, 0x0C);
    WriteRawRC(MFRC522_REG_BIT_FRAMING, ReadRawRC(MFRC522_REG_BIT_FRAMING) | (0x80)); //StartSend=1,transmission of data starts

    i=2000;

    do{
        n= ReadRawRC(MFRC522_REG_COMM_IRQ);
        i--;
    }while ((i!=0) && !(n&0x01) && !(n&0x30));

    WriteRawRC(MFRC522_REG_BIT_FRAMING, ReadRawRC(MFRC522_REG_BIT_FRAMING) | (~0x80)); //StartSend=0,transmission of data starts

    if (!(ReadRawRC(MFRC522_REG_ERROR) & 0x1B)) // Card detected
    {
        status=1;
        if (n&0x77&0x01)
        {
            status=0;
        }
    }

    n=ReadRawRC(MFRC522_REG_FIFO_LEVEL);

    len=n;
    i=0;

    while (n!=0)
    {
        buff_ID[i]= ReadRawRC(MFRC522_REG_FIFO_DATA);

        i++;
        n--;
    }

    return status;
}

int RFID::Receive_ID(uint8_t * buff_ID) // Retrieves card's ID
{
    char buff_cmd[2];
    int len=0;
    int status=0;

    buff_cmd[0]=0x26;
    len=1;
    WriteRawRC(MFRC522_REG_BIT_FRAMING,0x07); //  BitFramingReg Stars transmission
    status = Check_Card( buff_cmd, len, buff_ID);

    if (status==1 )
    {
        buff_cmd[0]=0x93;
        buff_cmd[1]=0x20;
        len=2;
        WriteRawRC(MFRC522_REG_BIT_FRAMING,0x00);
        status = Check_Card( buff_cmd, len, buff_ID);
    }
    else{
        status = 0;
    }

    return status;
}

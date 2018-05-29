#include "xbee.h"

XBee::XBee()
{
    int a, b;
    open_usart();
    sequence_request(&a, &b, 1);
    close_usart();
}

void XBee::open_usart()
{
    if ((fd = serialOpen("/dev/ttyAMA0",9600))<0)
    {
        cerr << "Unable to open serial device" << endl;
        exit(1);
    }
}

void XBee::close_usart()
{
    serialClose(fd);
}


void XBee::send_toXBee_IS()
{
    serialPutchar(fd, 0x7E);// Delimiter

    serialPutchar(fd, 0x00);// Lenght
    serialPutchar(fd, 0x0F);

    serialPutchar(fd, 0x17);// Frame typer

    serialPutchar(fd, 0x01);// frame ID

    serialPutchar(fd, 0x00);// destination address
    serialPutchar(fd, 0x13);
    serialPutchar(fd, 0xA2);
    serialPutchar(fd, 0x00);
    serialPutchar(fd, 0x40);
    serialPutchar(fd, 0x99);
    serialPutchar(fd, 0x32);
    serialPutchar(fd, 0x14);

    serialPutchar(fd, 0xFF); // 16- bit address
    serialPutchar(fd, 0xFE);

    serialPutchar(fd, 0x02); // Additional options

    serialPutchar(fd, 0x49); // IS commane
    serialPutchar(fd, 0x53);

    serialPutchar(fd, 0x78);// Checksum

}

void XBee::send_toXBee_LED(int i)
{
    int j=0;

    serialPutchar(fd, 0x7E);// Delimiter

    serialPutchar(fd, 0x00);// Lenght
    serialPutchar(fd, 0x10);

    serialPutchar(fd, 0x17);// Frame typer

    serialPutchar(fd, 0x01);// frame ID

    serialPutchar(fd, 0x00);// destination address
    serialPutchar(fd, 0x13);
    serialPutchar(fd, 0xA2);
    serialPutchar(fd, 0x00);
    serialPutchar(fd, 0x40);
    serialPutchar(fd, 0x99);
    serialPutchar(fd, 0x32);
    serialPutchar(fd, 0x14);

    serialPutchar(fd, 0xFF);// 16- bit address
    serialPutchar(fd, 0xFE);

    serialPutchar(fd, 0x02);// Additional options

    serialPutchar(fd, 0x44);//IS commane
    serialPutchar(fd, 0x30);

    if (i==1)
    {
        serialPutchar(fd, 0x05);
        serialPutchar(fd, 0x9B);// Checksum
    }
    else{

        serialPutchar(fd, 0x04);
        serialPutchar(fd, 0x9C);// Checksum
    }

    while (j<19) {
        serialGetchar(fd);
        j++;
    }
}

int XBee::read_toXBee(int *x, int*z)
{
    char MSB_x=0;
    char LSB_x=0;
    char MSB_z=0;
    char LSB_z=0;

    int i=0;

    memset(msg_XBee,'\0',sizeof(char)*30);
    if ((msg_XBee[i]=serialGetchar(fd)) == 0xff) // timeout
        return -1;

    if (msg_XBee[i++]==0x7E)
    {

        for (;i<=23;i++)
        {
            msg_XBee[i]= serialGetchar(fd);
        }

        MSB_x=serialGetchar(fd);
        LSB_x=serialGetchar(fd);
        MSB_z=serialGetchar(fd);
        LSB_z=serialGetchar(fd);

        *x=(int)((MSB_x<<8)|LSB_x);
        *z=(int)((MSB_z<<8)|LSB_z);
    }
    msg_XBee[i]= serialGetchar(fd);
    return 0;
}

int XBee::init_UART(const char *device, const int baud)
{
    struct termios options ;
    speed_t myBaud ;
    int     status, fd ;

    switch (baud)
    {
    case     50:	myBaud =     B50 ; break ;
    case     75:	myBaud =     B75 ; break ;
    case    110:	myBaud =    B110 ; break ;
    case    134:	myBaud =    B134 ; break ;
    case    150:	myBaud =    B150 ; break ;
    case    200:	myBaud =    B200 ; break ;
    case    300:	myBaud =    B300 ; break ;
    case    600:	myBaud =    B600 ; break ;
    case   1200:	myBaud =   B1200 ; break ;
    case   1800:	myBaud =   B1800 ; break ;
    case   2400:	myBaud =   B2400 ; break ;
    case   4800:	myBaud =   B4800 ; break ;
    case   9600:	myBaud =   B9600 ; break ;
    case  19200:	myBaud =  B19200 ; break ;
    case  38400:	myBaud =  B38400 ; break ;
    case  57600:	myBaud =  B57600 ; break ;
    case 115200:	myBaud = B115200 ; break ;
    case 230400:	myBaud = B230400 ; break ;

    default:
        return -2 ;
    }

    if ((fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
        return -1 ;

    fcntl (fd, F_SETFL, O_RDWR) ;

    // Get and modify current options:

    tcgetattr (fd, &options) ;

    cfmakeraw   (&options) ;
    cfsetispeed (&options, myBaud) ;
    cfsetospeed (&options, myBaud) ;
    options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    options.c_cflag |= (CLOCAL | CREAD) ;
    options.c_cflag &= ~(CSIZE) ;
    options.c_cflag |= CS8 ;
    options.c_lflag &= ~(ICANON | ECHO | ECHONL | IEXTEN | ISIG) ;
    options.c_oflag &= ~OPOST;

    options.c_cc [VMIN]  =   1 ;
    options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

    tcsetattr (fd, TCSAFLUSH, &options) ;

    ioctl (fd, TIOCMGET, &status);

    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;

    ioctl (fd, TIOCMSET, &status);

    usleep (10000) ;	// 10mS

    return fd ;
}

int XBee::sequence_request(int * res_x, int * res_z, int samp)
{
    int i=0;
    int x=0;
    int z=0;

    while (i!=samp)
    {
        send_toXBee_IS();

        if (read_toXBee(&x,&z) == -1)
            return -1;

#if DEBUG
        cout << "x: " << x << " z: " << z << endl;
#endif
        res_x[i]=x;
        res_z[i]=z;
        i++;
    }

    return 0;
}



#ifndef INPUT_H
#define INPUT_H
#include <string.h>
#include <mqueue.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "pthrd.h"
#include "rfid.h"
#include "xbee.h"
#include <iostream>
#include <QString>
#include <time.h>
#include "elock.h"
#include "stm.h"
#include <errno.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef SAMPLES
#define SAMPLES 20
#endif

#define RF              1
#define SPK				1
#define XBEE			1

using namespace std;

#ifndef    QCOL_NAME
#define    QCOL_NAME    "/queue_collect"
#endif

#ifndef    QSTA_NAME
#define    QSTA_NAME    "/queue_status"
#endif

/***** Semaphores descriptors  ********/
extern sem_t sem_Input;
extern sem_t sem_Rec;

//Struct for receiving and sending data through queues
struct buffs_i
{
    char c_buff[10];
    int i_buff[SAMPLES*2+1];
};

class Input: public  Pthrd
{
    //Private Variable of the Input Class
private:
    int final;
    int counter;
    struct timespec ts_inp;
    int ret_val;
    mqd_t q_inp;
    char c;
    int i;
    char str_dbg[10];
    int tries_counter;
    uint8_t buff_ID[5];
    uint8_t status;
    char buff_queue[10];
    int res_z[SAMPLES], res_x[SAMPLES*2+1];
    int msg_q;
    unsigned int sender;
    buffs_i msg_input;
    struct mq_attr msgq_attr;
    XBee *xb;

public:
    Input(XBee xb);//Constructor of this class, the receives an object of the class XBee
    void run(void);// virtual function run, to start running the Input_Pthread

};
#endif // INPUT_H

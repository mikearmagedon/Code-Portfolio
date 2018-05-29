#ifndef STATUS_H
#define STATUS_H

#include <mqueue.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "pthrd.h"
#include <string.h>
#include <time.h>
#include <QString>
#include <errno.h>
#include <iostream>

using namespace std;

#ifndef    QUI_NAME
#define    QUI_NAME    "/queue_ui"

#endif

#ifndef SAMPLES
#define SAMPLES 20
#endif

#ifndef    QSTA_NAME
#define    QSTA_NAME    "/queue_status"
#endif

extern sem_t sem_Rec;

struct buffs_s
{
    char c_buff[12];
    int i_buff[SAMPLES*2+1];
};

struct buffs_ui_sta
{
    char commd[3];
    QString msg;
    QString *last_entry;
};


class status: public Pthrd
{
private:
    /*Variable off Status class*/
    struct mq_attr atr;
    buffs_ui_sta msg_ui;
    int ret;
    mqd_t q_sta;
    buffs_s msg_sta;
    int msg_q;
    unsigned int sender;
    struct timespec ts_sta;
    char state[10];

public:
    status();//Constructor
    void run(void);// virtual function run, to start running the Status_Pthread
};

#endif // STATUS_H

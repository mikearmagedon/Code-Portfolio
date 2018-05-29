#ifndef COMPARE_H
#define COMPARE_H

#include <mqueue.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "pthrd.h"
#include "database.h"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <QSound>
#include <string>
#include <sstream>

using namespace std;

#ifndef SAMPLES
#define SAMPLES 20
#endif

#ifndef    SCM_NAME
#define    SCM_NAME    "sem_comp"

#endif

#ifndef    QCOL_NAME
#define    QCOL_NAME    "/queue_collect"

#endif

#ifndef    QCOM_NAME
#define    QCOM_NAME    "/queue_compare_db"

#endif

#ifndef    QUI_NAME
#define    QUI_NAME    "/queue_ui"

#endif

//#define DEBUG 1

/** Descriptor of the semaphore sem_Input**/
extern sem_t sem_Input;

struct buffs_c
{
    //char c_buff[2];
    char c_buff[11];
    int i_buff[SAMPLES*2+1];

};

struct buffs_ui_c
{
    char commd[3];
    char msg[100];
    char last_entry[20];
};


class Compare: public Pthrd
{
private:
    buffs_ui_c msg_ui;
    unsigned int x;
    char buff_aux[100];
    int j;
    int k;
    QString result;
    int cont_i;
    int cont_m;
    int cont_f;
    int cont_e;
    int acc_x[SAMPLES+1];
    int acc_z[SAMPLES+1];
    struct mq_attr msgq_attr;
    unsigned int sender;
    int i;
    mqd_t q_comp;
    int msg_q;
    buffs_c msg_comp;
    int buff_acc[100];
    char buff_queue[100];
    char db_queue[10];
    sem_t *sem_Compare;

public:
    Compare();// Constructor
    void run(void);// virtual function run, to start running the Input_Pthread
};

#endif // COMPARE_H

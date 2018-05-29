#include "mainwindow.h"
#include <QApplication>
#include "xbee.h"
#include <sys/stat.h>
#include <errno.h>
#include "input.h"
#include "compare.h"
#include "database.h"
#include <iostream>
#include "button.h"
#include "status.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef    QSTA_NAME
#define    QSTA_NAME    "/queue_status"
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

#ifndef    SCM_NAME
#define    SCM_NAME    "sem_comp"
#endif

mqd_t Queue_DB_Process;
mqd_t Queue_UI_msgs;
mqd_t Queue_system_status;
mqd_t Queue_collected_data;
mqd_t Queue_record_samples;

sem_t sem_Rec;
sem_t sem_Input;
sem_t* sem_Compare;

using namespace std;

int init_Queues()
{
    mqd_t queue_col;
    mqd_t queue_sta;
    mqd_t queue_ui;

    queue_sta = mq_open(QSTA_NAME , O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXG,NULL);
    if (queue_sta == (mqd_t)-1)
    {
        cerr << "Queue not created" << "errno: " << errno << endl;
        exit(1);
    }
    mq_close(queue_sta);

    queue_col = mq_open(QCOL_NAME , O_CREAT, S_IRWXU | S_IRWXG, NULL);
    if (queue_col == (mqd_t)-1)
    {
        cerr << "Queue not created" << "errno: " << errno << endl;
        exit(1);
    }
    mq_close(queue_col);

    queue_ui = mq_open(QUI_NAME , O_CREAT, S_IRWXU | S_IRWXG, NULL);
    if (queue_ui == (mqd_t)-1)
    {
        cerr << "Queue not created" << "errno: " << errno << endl;
        exit(1);
    }
    mq_close(queue_ui);

    return 0;
}

int init_Semaphores()
{
    sem_init(&sem_Input,0,0);
    sem_init(&sem_Rec,0,0);

    if ((sem_Compare = sem_open(SCM_NAME , O_CREAT, 0644, 0)) == SEM_FAILED) {
        cerr << "semaphore initilization" << endl;
        cerr << "errno: " << errno <<endl;
        sem_close(sem_Compare);
        sem_unlink(SCM_NAME);
        exit(1);
    }
    sem_close(sem_Compare);
    return 0;
}

int init_UI(int argc, char * argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

int main(int argc, char *argv[])
{
    Button bt;
    XBee xb;
    int pid;

    pid=fork();

    if (pid==0)
    {
        init_UI(argc,argv);

    }else{
        init_Queues();
        init_Semaphores();

        //Creating Input_Thread
        Input inp(xb);
        if (inp.Start(2))
        {
            cerr << "Couldn't' create Input_Thread" << endl;
            exit (1);
        }

        Compare comp;
        //Creating Compare_Thread
        if (comp.Start(1))
        {
            cerr << "Couldn't' create Compare_Thread" << endl;
            exit (1);
        }

        //Creating Status_Thread
        status sta;
        if (sta.Start(3))
        {
            cerr << "Couldn't' create Status_Thread" << endl;
            exit (1);
        }

        pthread_exit(NULL);
    }
}




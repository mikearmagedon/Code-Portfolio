#include "status.h"

pthread_cond_t condition_cond= PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

status::status()
{

}

void status::run()
{
    while (1)
    {
        memset(msg_sta.c_buff, '\0', sizeof(char)*12);
        q_sta = mq_open(QSTA_NAME, O_RDWR);

        if (q_sta == (mqd_t)-1)
        {
            cerr << "Error opening queue" << endl;
            exit(1);
        }

        //prepare time structure for mq_timedreceive()
        clock_gettime(CLOCK_REALTIME,&ts_sta);
        ts_sta.tv_sec += 1;
        ts_sta.tv_nsec += 1000;

        msg_q = mq_timedreceive(q_sta, (char*)&msg_sta,10000,&sender, &ts_sta);

        if (strcmp(msg_sta.c_buff,"RFID")==0 || strcmp(msg_sta.c_buff,"WRIST")==0 || strcmp(msg_sta.c_buff,"SPK")==0 )
        {
            strcpy(state,msg_sta.c_buff);
        }

        mq_close(q_sta);

        if (strcmp(msg_sta.c_buff, "SPK" )==0)
        {
            pthread_mutex_lock(&cond_mutex);
            pthread_cond_wait(&condition_cond,&cond_mutex);
            pthread_mutex_unlock(&cond_mutex);
        }

        //verify if the button was pressed during a certain period of time
        ret=sem_timedwait(&sem_Rec, &ts_sta);

        if (ret==0)
        {
            if (strcmp(state, "SPK" )!=0)
            {
                if (strcmp(state, "RFID" )==0)
                {
                    msg_ui.commd[0]='A';
                    msg_ui.commd[1]='R';

                    q_sta = mq_open(QUI_NAME, O_RDWR);

                    if (q_sta == (mqd_t)-1)
                    {
                        cerr << "UI:In mq_open()" << endl;
                        exit(1);
                    }

                    mq_send(q_sta,(const char*)&msg_ui,sizeof(msg_ui),0);

                    mq_close(q_sta);

                }
                else if (strcmp(state, "WRIST" )==0)
                {
                    msg_ui.commd[0]='A';
                    msg_ui.commd[1]='W';

                    q_sta = mq_open(QUI_NAME, O_RDWR);

                    if (q_sta == (mqd_t)-1)
                    {
                        cerr << "UI:In mq_open()" << endl;
                        exit(1);
                    }

                    mq_send(q_sta,(const char*)&msg_ui,sizeof(msg_ui),0);

                    mq_close(q_sta);
                }
            }
        }
        if (msg_sta.c_buff[0]=='T')
        {
            msg_ui.commd[0]='T';
            msg_ui.commd[1]='T';
            q_sta = mq_open(QUI_NAME, O_RDWR);

            if (q_sta == (mqd_t)-1)
            {
                cerr << "UI:In mq_open()" << endl;
                exit(1);
            }

            mq_send(q_sta,(const char*)&msg_ui,sizeof(msg_ui),0);

            mq_close(q_sta);
        }

        if (msg_sta.c_buff[0]=='E')
        {

            msg_ui.commd[0]='T';
            msg_ui.commd[1]='E';
            q_sta = mq_open(QUI_NAME, O_RDWR);

            if (q_sta == (mqd_t)-1)
            {
                cerr << "UI:In mq_open()" << endl;
                exit(1);
            }

            mq_send(q_sta,(const char*)&msg_ui,sizeof(msg_ui),0);

            mq_close(q_sta);
        }

        memset(msg_ui.commd, '\0', sizeof(int)*5);
        usleep(1000);
    }
}


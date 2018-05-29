#include "worker.h"

Worker::Worker(QObject *parent)
    :QObject(parent)
{
    sound_IDMNF = new QSound("/root/sounds/IDMNF.wav",this);
    sound_IDMF = new QSound("/root/sounds/IDMF.wav",this);
    sound_WMNF = new QSound("/root/sounds/WMNF.wav",this);
    sound_WMF = new QSound("/root/sounds/WMF.wav",this);
    sound_SNR = new QSound("/root/sounds/SNR.wav",this);
    sound_SR = new QSound("/root/sounds/SR.wav",this);
    sound_Timeout = new QSound("/root/sounds/Timeout.wav",this);
    sound_ARFID = new QSound("/root/sounds/ARFID.wav",this);
    sound_AWRIST = new QSound("/root/sounds/AWrist.wav",this);
    sound_Three= new QSound("/root/sounds/THREE.wav",this);
}

void Worker::process()
{    
    while (1){
        memset(msg_ui.commd,'\0',sizeof(char)*3);

        q_ui = mq_open(QUI_NAME, O_RDWR);

        if (q_ui == (mqd_t)-1)
        {
            cerr << "Error opening queue\n" << endl;
            exit(1);
        }

        msg_q = mq_receive(q_ui,(char*)&msg_ui,10000,&sender);
        if (msg_q == -1)
        {
            cerr << "Error Receiving" << endl;
            exit(1);
        }


        mq_close(q_ui);

        switch(msg_ui.commd[0])
        {
        case'R':
            if (msg_ui.commd[1]=='Y')
            {
                memset(msg_ui.commd,'\0',sizeof(char)*3);
#if DEBUG
                cout<<"IDMF"<<endl;
                cout << "msg_ui.msg: " << msg_ui.msg << endl;
                cout << "msg_ui.last: " << msg_ui.last_entry << endl;
#endif
                emit signal_RFID_MatchFound(msg_ui.msg,msg_ui.last_entry);
                sound_IDMF->play();
                while ((sound_IDMF->isFinished()==false));
                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
            }
            else if (msg_ui.commd[1]=='N')
            {
                emit signal_RFID_MatchNotFound();
                sound_IDMNF->play();
                while ((sound_IDMNF->isFinished()==false));

                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
                emit signal_Hide();
            }
            break;
        case 'S':

            if (msg_ui.commd[1]=='Y')
            {
                emit signal_Speech_Match("SR");
                sound_SR->play();
                while ((sound_SR->isFinished()==false));

                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
            }
            else if (msg_ui.commd[1]=='N')
            {
                emit signal_Speech_Match("SNR");
                sound_SNR->play();
                while ((sound_SNR->isFinished()==false));
                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
            }
            break;
        case 'W':
            if (msg_ui.commd[1]=='Y')
            {
                emit signal_WristMatch("WMF");
                sound_WMF->play();
                while ((sound_WMF->isFinished()==false));

                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
                delay(5000);
                emit signal_Hide();
            }
            else if (msg_ui.commd[1]=='N')
            {
                emit signal_WristMatch("WMNF");
                sound_WMNF->play();
                while ((sound_WMNF->isFinished()==false));

                if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                    cerr << "semaphore initilization error: Worker" << endl;
                }

                sem_post(sem_Compare);
                sem_close(sem_Compare);
            }

            break;

        case 'T':
            if (msg_ui.commd[1]=='T')
            {
                emit signal_Timeout("TIMOUT");
                sound_Timeout->play();
                while ((sound_Timeout->isFinished()==false));
                emit signal_Hide();
            }
            else if (msg_ui.commd[1]=='E')
            {
                emit signal_Timeout("THREE");
                sound_Three->play();
                while ((sound_Three->isFinished()==false));
                emit signal_Hide();
            }
            break;

        case 'A':
            if (msg_ui.commd[1]=='R')
            {
                emit signal_Alert("RFID");
                sound_ARFID->play();
                while ((sound_ARFID->isFinished()==false));
                emit signal_Hide();
            }
            else if (msg_ui.commd[1]=='W')
            {
                emit signal_Alert("WRIST");
                sound_AWRIST->play();
                while ((sound_AWRIST->isFinished()==false));
                emit signal_Hide();
            }
            break;
        }
    }
}

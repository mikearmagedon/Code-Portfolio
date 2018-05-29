#include "compare.h"

Compare::Compare()
{

}


void Compare::run()
{
    Database  db("/root/so.db");
    if (db.connect()==false)
    {
        exit(1);
    }

    while (1)
    {
        memset(buff_queue,'\0',sizeof(char)*100);

        // Reads queue and procceds to the proper verification method
        q_comp = mq_open(QCOL_NAME, O_RDWR);

        if (q_comp == (mqd_t)-1)
        {
            cerr << "Error opening queue\n" << endl;
            exit(1);
        }

        msg_q = mq_receive(q_comp, (char*)&msg_comp,10000,&sender);//Must send the received value for verification
        if (msg_q == -1)
        {
            cerr << "Error Receiving" << endl;
            exit(1);
        }

        mq_close(q_comp);
#if DEBUG
        i=0;
        while (i<11)
        {
            cout<<"COMP:" << msg_comp.c_buff[i]<< "i:"<<i<<endl;
            i++;
        }
        cout<<msg_comp.c_buff[0]<<endl;
        cout << "Compare received: " << msg_comp.c_buff[0] << msg_comp.i_buff[0] << hex << buff_queue[2] << buff_queue[3] << buff_queue[4] << buff_queue[5] << dec << endl;
#endif
        switch(msg_comp.c_buff[0])
        {
        case'R':
        {
            memset(db_queue,'\0',sizeof(char)*10);
            int q;

            for (q=0; q<10; q++)
            {
                db_queue[q]=msg_comp.c_buff[q+1];
            }

            //Goes to database and verifies if there is a RFID user

            QString lastEntry;
            result = db.requestRFID(db_queue, &lastEntry);

            if (result == "")
            {
                msg_ui.commd[0]='N';

            }else {
                msg_ui.commd[0]='Y';
                strcpy(msg_ui.msg, result.toStdString().c_str());
                strcpy(msg_ui.last_entry, lastEntry.toStdString().c_str());
#if DEBUG
                cout<<"msg_ui.msg: " << msg_ui.msg<<endl;
                cout<<"msg_ui.last_entry: " << msg_ui.last_entry<<endl;
#endif
            }

            if (msg_ui.commd[0]=='Y')
            {                
                q_comp = mq_open(QCOL_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "In mq_open()" << endl;
                    exit(1);
                }

                msg_comp.c_buff[0]='Y';
                mq_send(q_comp,(const char*)&msg_comp,sizeof(msg_comp),0);

                mq_close(q_comp);

                msg_ui.commd[0]='R';
                msg_ui.commd[1]='Y';

                msg_ui.commd[2]='\0';

                q_comp = mq_open(QUI_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "UI:In mq_open()" << endl;
                    exit(1);
                }
#if DEBUG
                cout<<msg_ui.msg<<endl;
#endif
                mq_send(q_comp,(const char*)&msg_ui, sizeof(msg_ui), 0);

                mq_close(q_comp);

            }else if (msg_ui.commd[0]=='N')
            {

                q_comp = mq_open(QCOL_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "In mq_open()" << endl;
                    exit(1);
                }
                msg_comp.c_buff[0]='N';
                mq_send(q_comp,(const char*)&msg_comp,sizeof(msg_comp),0);

                mq_close(q_comp);

                msg_ui.commd[0]='R';
                msg_ui.commd[1]='N';

                q_comp = mq_open(QUI_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "UI:In mq_open()" << endl;
                    exit(1);
                }

                mq_send(q_comp,(const char*)&msg_ui,sizeof(msg_ui),0);

                mq_close(q_comp);
            }

            if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                cerr << "semaphore initilization error: Compare" << endl;
            }
            sem_wait(sem_Compare);
            sem_post(&sem_Input);
            sem_close(sem_Compare);
        }
            break;

        case'S':
            if (db.requestVoice().toStdString()[0]==msg_comp.c_buff[1])
            {
                buff_queue[0]='Y';
                msg_ui.commd[0]='S';
                msg_ui.commd[1]='Y';
                msg_ui.commd[2]='\0';

            }else {

                buff_queue[0]='N';
                msg_ui.commd[0]='S';
                msg_ui.commd[1]='N';
                msg_ui.commd[2]='\0';
            }

            q_comp = mq_open(QCOL_NAME, O_RDWR);

            if (q_comp == (mqd_t)-1)
            {
                cerr << "In mq_open()" << endl;
                exit(1);
            }

            mq_send(q_comp,buff_queue,strlen(buff_queue)+1,0);

            mq_close(q_comp);

            q_comp = mq_open(QUI_NAME, O_RDWR);

            if (q_comp == (mqd_t)-1)
            {
                cerr << "UI:In mq_open()" << endl;
                exit(1);
            }

            mq_send(q_comp,(const char*)&msg_ui,sizeof(msg_ui),0);

            mq_close(q_comp);

            if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                cerr << "semaphore initilization error: Compare" << endl;
            }

            sem_wait(sem_Compare);
            sem_post(&sem_Input);
            sem_close(sem_Compare);
            break;

        case'W':
            i=0;
            while (i<SAMPLES)
            {
                acc_x[i]=msg_comp.i_buff[i];
#if DEBUG
                cout<<"acc_x: "<<acc_x[i]<<endl;
#endif
                i++;
            }
            while (i<2*SAMPLES)
            {
                acc_z[i-SAMPLES]=msg_comp.i_buff[i];
#if DEBUG
                cout<<"acc_z: "<<acc_z[i-SAMPLES]<<" i:"<<i<<endl;
#endif
                i++;
            }

            result = db.requestPattern();

            i=0;
            j=0;
            k=0;
            int aux;
            aux=0;

            while (k<12)
            {
                aux=i*3;
                sscanf(result.toStdString().substr(aux,3).c_str(),"%x",&buff_acc[j]);
                i++;
                j++;
                k++;
            }

            memset(buff_aux, '\0', sizeof(int)*4);
            i=0;
            cont_f=0;
            cont_e=0;
            cont_m=0;
            cont_i=0;
            while (i<2*SAMPLES)//Pattern verification
            {
                //initial position
                if ((acc_x[i]>=buff_acc[0] && acc_x[i]<buff_acc[1]) && (acc_z[i]>=buff_acc[2] && acc_z[i] <=buff_acc[3]) )
                {
                    cont_i++;
                }//middle position
                else if ( (acc_x[i]>buff_acc[4] && acc_x[i]<buff_acc[5]) && (acc_z[i]>buff_acc[6] && acc_z[i] < buff_acc[7]) )
                {
                    cont_m++;
                }//Final position
                else if ( (acc_x[i]>buff_acc[8] && acc_x[i]<buff_acc[9]) && (acc_z[i]>buff_acc[10] && acc_z[i] < buff_acc[11]) )
                {
                    cont_f++;
                }
                else {
                    cont_e++;
                }
                i++;
            }
#if DEBUG
            cout<<"i:"<<cont_i<<endl;
            cout<<"m:"<<cont_m<<endl;
            cout<<"f:"<<cont_f<<endl;
            cout<<"e:"<<cont_e<<endl;
#endif
            if (cont_i!=0 && cont_f!=0 && cont_m!=0)
            {                
                buff_queue[0]='Y';

                q_comp = mq_open(QCOL_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "In mq_open()" << endl;
                    exit(1);
                }

                mq_send(q_comp,buff_queue,strlen(buff_queue)+1,0);

                mq_close(q_comp);

                msg_ui.commd[0]='W';
                msg_ui.commd[1]='Y';
                msg_ui.commd[2]='\0';

                q_comp = mq_open(QUI_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "UI:In mq_open()" << endl;
                    exit(1);
                }

                mq_send(q_comp,(const char*)&msg_ui,sizeof(msg_ui),0);

                mq_close(q_comp);

                if (db.insertEntry()) // log user entry
                {
#if DEBUG
                    cout << "Entry logged succesfully" << endl;
#endif
                }else{
#if DEBUG
                    cout << "Error logging entry" << endl;
#endif
                }
            }else
            {
                buff_queue[0]='N';
                q_comp = mq_open(QCOL_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "In mq_open()" << endl;
                    exit(1);
                }

                mq_send(q_comp,buff_queue,strlen(buff_queue)+1,0);

                mq_close(q_comp);

                msg_ui.commd[0]='W';
                msg_ui.commd[1]='N';
                msg_ui.commd[2]='\0';
                q_comp = mq_open(QUI_NAME, O_RDWR);

                if (q_comp == (mqd_t)-1)
                {
                    cerr << "UI:In mq_open()" << endl;
                    exit(1);
                }

                mq_send(q_comp,(const char*)&msg_ui,sizeof(msg_ui),0);

                mq_close(q_comp);
            }
            if ((sem_Compare = sem_open(SCM_NAME , 0)) == SEM_FAILED) {
                cerr << "semaphore initilization error: Compare" << endl;
            }

            sem_wait(sem_Compare);
            sem_post(&sem_Input);
            sem_close(sem_Compare);
            break;

        default:
            break;
        }
    }
}

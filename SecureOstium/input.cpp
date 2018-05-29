#include "input.h"

extern pthread_cond_t condition_cond;
extern pthread_mutex_t cond_mutex;

Input::Input(XBee xb)
{
    this->xb = &xb;
}

void Input::run()
{
    ELock el;
    RFID rf;
    XBee xb;
    final=0;
    STM stm;

    rf.init_MFRC522();
    
    while (1)
    {
        //        if (final ==1)
        //        {
        //            //Timer for 5 seconds

        //        }
        memset(buff_queue,'\0',sizeof(char)*10);
        
#if RF
        strcpy(msg_input.c_buff, "RFID");

        q_inp = mq_open(QSTA_NAME, O_RDWR);

        if (q_inp == (mqd_t)-1)
        {
            cerr << "In mq_open()" << endl;
            exit(1);
        }

        mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);

        mq_close(q_inp);

        memset(msg_input.c_buff,'\0',sizeof(char)*10);

        while (buff_queue[0]!='Y')
        {
            //UPDATES CURRENT STATE
            // Starts reading SPI where fetches user card's ID
            status = rf.Receive_ID(buff_ID);
            if (status == 1 && ((((buff_ID[0]!=buff_ID[1])!=buff_ID[2])!=buff_ID[3])!=buff_ID[4])!=0x00)
            {
#if DEBUG
                //sprintf(str_dbg, "%c%c%c%c%c" ,buff_ID[0],buff_ID[1],buff_ID[2],buff_ID[3],buff_ID[4] );
                //cout << "Card: " << hex << buff_ID[0] << buff_ID[1] << buff_ID[2] << buff_ID[3] << buff_ID[4] << dec << endl;
#endif   
                sprintf(msg_input.c_buff, "%c%x%x%x%x%x" ,'R',buff_ID[0],buff_ID[1],buff_ID[2],buff_ID[3],buff_ID[4] );

                //Sends for verification
                q_inp = mq_open(QCOL_NAME, O_RDWR);
                
                if (q_inp == (mqd_t)-1)
                {
                    cerr << "In mq_open()" << endl;
                    exit(1);
                }
                
                mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);

                mq_close(q_inp);

                sem_wait(&sem_Input);

                q_inp = mq_open(QCOL_NAME, O_RDWR);
                
                if (q_inp == (mqd_t)-1)
                {
                    cerr << "Error opening queue\n" << endl;
                    exit(1);
                }
                
                mq_getattr(q_inp,&msgq_attr);

                msg_q = mq_receive(q_inp, buff_queue,10000,&sender);
                
                if (msg_q == -1)
                {
                    cerr << "Error Receiving" << endl;
                    exit(1);
                }
                
                mq_close(q_inp);
            }
        }
#endif

#if SPK
        if (buff_queue[0]=='Y')
        {
            counter=0;
            memset(buff_queue,'\0',sizeof(char)*10);

            strcpy(msg_input.c_buff, "SPK");

            q_inp = mq_open(QSTA_NAME, O_RDWR);

            if (q_inp == (mqd_t)-1)
            {
                cerr << "In mq_open()" << endl;
                exit(1);
            }

            mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);

            mq_close(q_inp);

            memset(buff_queue,'\0',sizeof(char)*10);
            while (buff_queue[0]!='Y' && counter<3 && buff_queue[0]!='T')
            {
                int ret=0;
                char c;
                c=0xDD;

                clock_gettime(CLOCK_REALTIME,&ts_inp);
                ts_inp.tv_sec += 10;
                ts_inp.tv_nsec += 0;

                ret=sem_timedwait(&sem_Rec, &ts_inp);// wait for the semaphore
                if (ret!=0)
                {
                    if (errno == ETIMEDOUT )
                    {
                        memset(buff_queue,'\0',sizeof(char)*10);
                        buff_queue[0]='T';

                        q_inp = mq_open(QSTA_NAME, O_RDWR);

                        if (q_inp == (mqd_t)-1)
                        {
                            cerr << "In mq_open()" << endl;
                            exit(1);
                        }

                        msg_input.c_buff[0]='T';
                        mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);
                        mq_close(q_inp);
                    }
                }
                else{
                    if (stm.openSPI(2)<0)
                    {
                        cerr << "Unable to open SPI device" << endl;
                        exit(1);
                    }

                    stm.WriteRawRC(0xAF);
                    do
                    {
                        c=stm.ReadRawRC();
#if DEBUG
                        cout << "C: " << hex << c << dec << endl;
#endif
                    }while ((c!=0xFE)&&(c!=0xFD)&&(c!=0xFC));

                    stm.closeSPI();
                    switch(c)
                    {
                    case 0xFE:
                        c='2';
                        break;
                    case 0xFD:
                        c='1';
                        break;
                    case 0xFC:
                        c='3';
                        break;
                    }

                    msg_input.c_buff[1]=c;
                    //Must send the resul to be compared
                    msg_input.c_buff[0]='S';

                    q_inp = mq_open(QCOL_NAME, O_RDWR);
                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "In mq_open()" << endl;
                        exit(1);
                    }
                    mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);
                    mq_close(q_inp);

                    sem_wait(&sem_Input);

                    q_inp = mq_open(QCOL_NAME, O_RDWR);
                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "Error opening queue\n" << endl;
                        exit(1);
                    }

                    mq_getattr(q_inp,&msgq_attr);
                    msg_q = mq_receive(q_inp, buff_queue,10000,&sender);
                    if (msg_q == -1)
                    {
                        cerr << "Error Receiving" << endl;
                        exit(1);
                    }
                    mq_close(q_inp);
                    counter++;
                }
                if (counter==3 && buff_queue[0]=='N')
                {
                    q_inp = mq_open(QSTA_NAME, O_RDWR);

                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "In mq_open()" << endl;
                        exit(1);
                    }
                    msg_input.c_buff[0]='E';
                    mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);
                    mq_close(q_inp);
                }
            }
            pthread_mutex_lock(&cond_mutex);
            pthread_cond_signal(&condition_cond);
            pthread_mutex_unlock(&cond_mutex);
        }
#endif
        
#if XBEE        
        ret_val=0;
        tries_counter=0;

        strcpy(msg_input.c_buff, "WRIST");

        q_inp = mq_open(QSTA_NAME, O_RDWR);

        if (q_inp == (mqd_t)-1)
        {
            cerr << "In mq_open()" << endl;
            exit(1);
        }

        mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);

        mq_close(q_inp);

        if (buff_queue[0]=='Y')
        {
            memset(buff_queue, '\0', sizeof(char)*50);
            strcpy(buff_queue, "WRIST");

            while (buff_queue[0]!='Y' && tries_counter<3 && buff_queue[0]!='T')
            {
                xb.open_usart();
                memset(res_x, '\0', sizeof(int)*SAMPLES*2);
                memset(res_z, '\0', sizeof(int)*SAMPLES);
                while (( *res_x <= 0x222 || *res_x > 0x23B ) || ( *res_z <= 0x2B1 || *res_z > 0x2B5 )) {
                    if ((ret_val = xb.sequence_request(res_x,res_z, 1))==-1)
                    {
                        break;
                    }
                }
                
                if (ret_val!=-1)
                {
                    xb.send_toXBee_LED(1);
                    // Request Samples from the Accelerometer
                    ret_val = xb.sequence_request(res_x, res_z, SAMPLES);

                    xb.send_toXBee_LED(0);
                    xb.close_usart();

                    buff_queue[0]='W';//Timer for 2 secs
                    i=0;
                    while (i<SAMPLES)
                    {
                        res_x[SAMPLES+i]= res_z[i];
                        i++;
                    }

                    memcpy(msg_input.i_buff, res_x,sizeof(int)*(SAMPLES*2));

                    msg_input.c_buff[0]='W';

                    q_inp = mq_open(QCOL_NAME, O_RDWR);
                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "In mq_open()" << endl;
                        exit(1);
                    }
                    mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);
                    mq_close(q_inp);
                    
                    sem_wait(&sem_Input);

                    q_inp = mq_open(QCOL_NAME, O_RDWR);
                    
                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "Error opening queue\n" << endl;
                        exit(1);
                    }

                    msg_q = mq_receive(q_inp, buff_queue,10000,&sender);
                    
                    if (msg_q == -1)
                    {
                        cerr << "Error Receiving" << endl;
                        exit(1);
                    }

                    mq_close(q_inp);

                    tries_counter++;
                    if (buff_queue[0]=='Y')
                    {
                        tries_counter=0;
                    }
                } else if (ret_val == -1)
                {
                    memset(buff_queue,'\0',sizeof(char)*10);
                    buff_queue[0]='T';

                    q_inp = mq_open(QSTA_NAME, O_RDWR);

                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "In mq_open()" << endl;
                        exit(1);
                    }

                    msg_input.c_buff[0]='T';
                    mq_send(q_inp,(const char*)&msg_input,sizeof(msg_input),0);
                    mq_close(q_inp);

                }
                if (tries_counter==3)
                {
                    buff_queue[0]='E';
                    
                    q_inp = mq_open(QSTA_NAME, O_RDWR);// queue for system status
                    
                    if (q_inp == (mqd_t)-1)
                    {
                        cerr << "In mq_open()" << endl;
                        exit(1);
                    }
                    
                    mq_send(q_inp,buff_queue,strlen(buff_queue)+1,0);// After there tries must send timeout to the Status_Tread
                    
                    mq_close(q_inp);
                }
            }
        }
        if (buff_queue[0]=='Y')
        {
            final=1;
            
            el.open();
            delay(5000);
            el.close();
        }
#endif
    }
}

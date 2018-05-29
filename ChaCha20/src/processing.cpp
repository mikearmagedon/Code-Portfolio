#include "processing.h"

char Processing::src_queue_name[20] = {0};
char Processing::dst_queue_name[20] = {0};

Processing::Processing(const uint8_t key[32], const uint8_t nonce[12])
: chacha(key, nonce)
{
}

void Processing::run(){
    int msg_proc;

    sigemptyset(&mask);
    sigaddset(&mask, RTSIGNAL_P);
    sigaddset(&mask, SIGINT);

    while (!quit)
    {
        int sig = sigwaitinfo(&mask, &info);
        if (sig == -1)
        {
            cerr << "\nPro:sigwaitinfo: " << strerror(errno) << endl;
            exit(1);
        }
        else if (sig == SIGINT)
        {
            quit = 1;
        }
        else if (sig == RTSIGNAL_P)
        {
            sprintf(src_queue_name, "/queue_prod%d", info.si_value.sival_int);
            sprintf(dst_queue_name, "/queue_cons%d", info.si_value.sival_int);
            queue_prod_proc = mq_open(src_queue_name, O_RDWR | O_NONBLOCK);

            if (queue_prod_proc == (mqd_t)-1)
            {
                cerr << "\nPro:Error opening queue " << strerror(errno) << endl;
                exit(1);
            }

            msg_proc = mq_receive(queue_prod_proc, reinterpret_cast<char*>(&msg_pross), sizeof(msg_pross), 0);
            if (msg_proc == -1)
            {
                cerr << "\nPro:Error Receiving " << strerror(errno) << endl;
                exit(1);
            }

            mq_close(queue_prod_proc);

            chacha.crypt(&msg_pross.bytes[0], msg_pross.numb_bytes_read);

            queue_cons_proc = mq_open(dst_queue_name , O_RDWR | O_NONBLOCK);

            if (queue_cons_proc == (mqd_t)-1)
            {
                cerr << "\nPro:In mq_open() " << strerror(errno) << endl;
                exit(1);
            }

            mq_send(queue_cons_proc,reinterpret_cast<const char*>(&msg_pross),sizeof(msg_pross),0);
            mq_close(queue_cons_proc);
        }
    }
    pthread_exit(NULL);
}

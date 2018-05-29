#include "consumer.h"

Consumer::Consumer()
{
	this->msg_con_int = -1;
	this->queue_cons = -1;
	this->filename = NULL;
    this->attr.mq_maxmsg = 50; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_cons); //Max size of message
    this->ID = -1;
}

Consumer::~Consumer()
{
    if (mq_unlink(queue_name) == -1) {
        cerr << "\nC:mq_unlink" << strerror(errno) << endl;
        exit(1);
    }
}

void Consumer::set_ID(int id) {
    this->ID = id;
}

void Consumer::set_filename(char * filename) {
    this->filename = filename;
}

void Consumer::run()
{
    if (ID == -1)
    {
        cerr << "\nP:Invalid thread ID:" << ID << endl;
        exit(1);
    }

    sprintf(queue_name, "/queue_cons%d", ID);

    queue_cons = mq_open(queue_name , O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if (queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Not created " << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_cons);

    queue_cons = mq_open(queue_name, O_RDWR);

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Error opening queue " << strerror(errno) << endl;
        exit(1);
    }

    ofstream fp;
    char name[strlen(filename)+6];
    strcpy(name, filename);
    strcat(name, ".rslt");
    
    if (ifstream(name))
    {
    	remove(name);
    }

    fp.open(name, ofstream::out | ios::binary | ofstream::app);

    do {
        msg_con_int = mq_receive(queue_cons, reinterpret_cast<char*>(&msg_cons),sizeof(msg_cons),0);
        if (msg_con_int == -1)
        {
            cerr << "\nC:Error Receiving " << strerror(errno) << endl;
            exit(1);
        }
        
        fp.write(reinterpret_cast<const char*>(&msg_cons.bytes[0]), msg_cons.numb_bytes_read);
    } while (--(msg_cons.no_of_blocks) > 0);

    fp.close();
    mq_close(queue_cons);

    pthread_exit(NULL);
}

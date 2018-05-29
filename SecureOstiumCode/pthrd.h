#ifndef PTHRD_H
#define PTHRD_H
#include <mqueue.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

#ifndef DEBUG
#define DEBUG 0
#endif

using namespace std;

class Pthrd
{
protected:
    int thread_policy;
    pthread_attr_t thread_attr; // Attribute of Pthread
    struct sched_param thread_param;
    pthread_t pthread;// object of type Pthread

private:
    virtual void run() = 0;// run virtual function
    static void* Pthrd_Func(void* );//Pthread function to call run fucntion

protected:
    //Setting up the attributes, the type of scheduler and the priority of pthread
    void SetupThread(int prio, pthread_attr_t *pthread_attr, struct sched_param * pthread_param);

public:
    int Start(int prio);//Function to start the thread process
    Pthrd();// Constructor
};

#endif // PTHRD_H

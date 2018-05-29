#include "pthrd.h"

Pthrd::Pthrd()
{

}

int Pthrd::Start(int prio)
{
    pthread_attr_init(&thread_attr);
    pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
    pthread_attr_getschedparam(&thread_attr, &thread_param);

    this->SetupThread(prio, &thread_attr, &thread_param);

    int handler = pthread_create(&pthread,&thread_attr,Pthrd_Func,static_cast<void*>(this));
#if DEBUG
    cout << "Priority: " << thread_param.__sched_priority << endl;
#endif
    pthread_detach(pthread);
    return handler;
}


void Pthrd::SetupThread(int prio, pthread_attr_t *pthread_attr, struct sched_param * pthread_param)
{
    // Step 3: Calculate and/or assign priority value to sched_param structure
    pthread_param-> sched_priority = prio;

    //Step 4: set scheduling policy
    pthread_attr_setschedpolicy (pthread_attr,SCHED_RR);

    //Step 5: set scheduling param
    pthread_attr_setschedparam(pthread_attr,pthread_param);

    //Step 6: set scheduling attributes to be determined by attributes object
    pthread_attr_setinheritsched(pthread_attr, PTHREAD_EXPLICIT_SCHED);
}

void* Pthrd::Pthrd_Func(void *p)
{
    Pthrd *pthrd = static_cast<Pthrd*>(p);
    pthrd->run();

    return 0;
}


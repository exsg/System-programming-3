#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <signal.h>

#include <string>


using namespace std;


class WorkerThread {
public:
    int id;
    pthread_t tid;
};

#endif 


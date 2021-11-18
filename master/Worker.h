#ifndef WORKER_H
#define WORKER_H

#include <signal.h>

#include <string>

using namespace std;


class Worker {
public:
    int id;
    pid_t pid;
    int w_to_a;
    int a_to_w;
    char w_to_a_name[8000];
    char a_to_w_name[8000];
};

#endif /* WORKER_H */


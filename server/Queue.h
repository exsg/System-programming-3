#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>


class Queue {
public:
    Queue(int size);
    ~Queue();
    
    void place(void* data);
    void* obtain();
private:
    void* * data;
    int start;
    int end;
    int count;
    int size;

    pthread_mutex_t mtx;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;
};

#endif /* QUEUE_H */


#include "Queue.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

using namespace std;

Queue::Queue(int size) : size(size) {
    start = 0;
    end = -1;
    count = 0;
    
    data = new void*[size];

    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);
    
    cout << "Queue created \n";

}

Queue::~Queue() {
    delete [] data;
    pthread_cond_destroy(&cond_nonempty);
    pthread_cond_destroy(&cond_nonfull);
    pthread_mutex_destroy(&mtx);
}

void Queue::place(void* d) {
    pthread_mutex_lock(&mtx);
    while (count >= size) {
        pthread_cond_wait(&cond_nonfull, &mtx);
    }
    end = (end + 1) % size;
    data[end] = d;
    count++;
    pthread_mutex_unlock(&mtx);

    pthread_cond_signal(&cond_nonempty);
}

void* Queue::obtain() {
    void* d = 0;
    pthread_mutex_lock(&mtx);
    while (count <= 0) {
        pthread_cond_wait(&cond_nonempty, &mtx);
    }
    d = data[start];
    start = (start + 1) % size;
    count--;
    pthread_mutex_unlock(&mtx);

    pthread_cond_signal(&cond_nonfull);
    return d;
}

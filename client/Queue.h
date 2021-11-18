#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <string>

using namespace std;

class Node{
    public:
        Node();
        Node(string data);
        // ~Node();
        string data;
        Node* nextNode;
       
};

class Queue{
    public:
    Queue();
    // ~Queue();
    Node *firstNode;
    int getSize();
    void QueueAdd(string data);
    string QueueGetFirstString();
    void itterateQueue();
    int size;
};

#endif
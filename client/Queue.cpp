#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

Node::Node(){
    nextNode=NULL;
    cout<<"node created WITHOUT data\n";
}
Node::Node(string data){
    
    this->data=data;
    nextNode=NULL;
    cout<<"node created with "<<this->data;
}

//Node::~Node

Queue::Queue(){
    firstNode=NULL;
    size=0;
    cout<<"Queue created with NULL for first node\n";
}

int Queue::getSize(){
    return size;
}

// Queue::~Queue()

void Queue::QueueAdd(string data){
    cout<<"vazw stoixeio sthn queue\n";
    if(firstNode==NULL){
        // cout<<"trying to create a node from QueueAdd\n";
        Node* tNode= new Node(data);
        firstNode=tNode;
        size++;
    }else{
        // cout<<"trying to create a new node because first element is not null\n";
        Node * tNode2=firstNode;
        while(tNode2->nextNode!=NULL){
            // cout<<"1\n";
            tNode2=tNode2->nextNode;
        }
        // cout<<"gia na doume an vgainei apoto while\n";
        tNode2->nextNode=new Node(data);
        size++;
    }

}

string Queue::QueueGetFirstString(){
    //delete data of the first node
    //do not forget
    string returnVal;
    if(firstNode!=NULL){
        returnVal=firstNode->data;
        firstNode=firstNode->nextNode;
        size--;
    }else{
        cout<<"Queue is empty\n";
        return "";
    }
    return returnVal;   
}


void Queue::itterateQueue(){
    Node* tNode;
    tNode=firstNode;
    while(tNode!=NULL){
        cout<<"itteration "<<tNode->data<<endl;
        tNode=tNode->nextNode;
    }
    cout<<"end of Queue\n";
}



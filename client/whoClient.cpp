#include <iostream>
#include <string.h>
#include <string>
#include "whoClientRead.h"
#include "Queue.h"
#include "Protocol.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>      /* sockets */
#include <sys/socket.h>      /* sockets */
#include <netinet/in.h>      /* internet sockets */
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>


using namespace std;

int counter = 0; //counter gia ola ta threads
int wakeup = 0;
int servPort;
string servIP;

pthread_mutex_t mvar, displayvar;
pthread_cond_t cvar_child, cvar_parent;

void * startRoutine(void *argp);

int main(int argc, char *argv[]) {
    //./whoClient -q queryFile -w 5 -sp 5 -sip 127.0.0.1
    // g++ -o whoClient -pthread  whoClient.cpp whoClientRead.cpp whoClientRead.h Queue.cpp Queue.h
    //./whoClient -q queryFile -w numThreads -sp servPort -sip servIP
    //./whoClient -q queryFile -w 5 -sp 8888 -sip 192.168.1.10
    //./whoClient -q queryFile -w 5 -sp 9999 -sip 192.168.1.10
    string queryFile;
    int numThreads;


    if (argc == 9) {

        int whichArg = 1;

        while (whichArg < 9) {
            if (strcmp(argv[whichArg], "-q") == 0) {
                queryFile = string(argv[whichArg + 1]);
                whichArg = whichArg + 2;
            } else if (strcmp(argv[whichArg], "-w") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    numThreads = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    cout << " provlhma sto -w\n";
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-sp") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    servPort = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    cout << " provlhma sto -sp\n";
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-sip") == 0) {
                servIP = string(argv[whichArg + 1]);
                whichArg = whichArg + 2;
            } else {
                printf("Error : inputs are wrong , ./whoClient -q queryFile -w numThreads -sp servPort -sip servIP\n");
                exit(-1);
            }
        }
    } else {
        // printArgumentError();
        cout << "Less or More arguments given\n";
        printf("Error : inputs are wrong , ./whoClient -q queryFile -w numThreads -sp servPort -sip servIP\n");

        exit(-1);
    }
    //./whoClient –q queryFile -w numThreads –sp servPort –sip servIP

    cout << " queryFile " << queryFile << " numThreads " << numThreads << " servPort " << servPort << " servIP " << servIP << endl;


    string* queriesArray=NULL;
    int index=0;
    // queriesArray=(StringArray)malloc(sizeof(StringArray));


    queriesArray = clientReadFile(queryFile , &index);
    // cout<<"from main 8esh sth mnhmh "<<queriesArray.size<<endl;

    for (int j = 0; j < index; j++) {
        cout << queriesArray[j];
    }

    cout << "these shits needs to be given to " << numThreads << " threads\n";

    for (int j = 0; j < index; j++) {
        printf("to %d query 8a to parei h %d diergasia\n", j, (j % numThreads));
    }

    pthread_mutex_init(&mvar, NULL); /* Initialize mutex */
    pthread_mutex_init(&displayvar, NULL); /* Initialize mutex */
    pthread_cond_init(&cvar_child, NULL);
    pthread_cond_init(&cvar_parent, NULL);


    int* thread_ids = (int*) malloc(sizeof (int)*numThreads);
    for (int i = 0; i < numThreads; i++) {
        thread_ids[i] = i;
    }

    for (int i = 0; i < numThreads; i++) {
        cout << "threads ids are " << thread_ids[i] << endl;
    }
    //===================================================================ftiaxnw queueArray me queries + emfanizw
    // QueueNode* thisQ=new QueueNode(&x);
    Queue myQ[numThreads];

    //mporei na xreiazetai +1 sto querisArray.size  j < queriesArray.size+1, pantws douleuei swsta
    for (int j = 0; j < index; j++) {
        myQ[j % numThreads].QueueAdd(queriesArray[j]);
    }

    cout << "itterateQueue , 8a vgalei gia ka8e oura ola ta stoixeia ths\n\n";
    for (int j = 0; j < numThreads; j++) {
        myQ[j].itterateQueue();
    }

    cout << "telos itterateQueue\n\n";
    //===================================================================

    cout << "===================================================================\n";
    cout << "                     Main thread, before pthread_create\n";
    cout << "===================================================================\n";
    pthread_t threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, startRoutine, &myQ[i]);
    }

    pthread_mutex_lock(&mvar);
    while (counter < numThreads) {
        pthread_cond_wait(&cvar_parent, &mvar);
    }
    pthread_mutex_unlock(&mvar);

    wakeup = 1;
    pthread_cond_broadcast(&cvar_child);

    cout << "Parent waits for children \n";

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // free(queriesArray.sArray);

    pthread_mutex_destroy(&mvar);
    pthread_mutex_destroy(&displayvar);
    pthread_cond_destroy(&cvar_child);
    pthread_cond_destroy(&cvar_parent);

    cout << "Parent exits \n";
    return 0;
}

void * startRoutine(void *argp) {
    Queue* queue = (Queue*) argp;

    pthread_mutex_lock(&mvar);
    counter++;
    pthread_mutex_unlock(&mvar);

    pthread_cond_signal(&cvar_parent);

    pthread_mutex_lock(&mvar);
    // cout << "thread waiting for signal " << (int*) argp << endl;
    cout << "thread function must print list\n ";

    //communication
    //den 3erw pws na to kanw na einai sugxronismeno , an valw mutex , otan 8a 3ekinhsei to ena thread
    //8a prepei na ta kanei ola etsi opws to exw. 
    while (wakeup == 0) {
        pthread_cond_wait(&cvar_child, &mvar);
    }

    pthread_mutex_unlock(&mvar);

    int iii = queue->getSize();
    for (int i = 0; i < iii; i++) {
        string printString = queue->QueueGetFirstString();
        cout << pthread_self() << ", apo QueueGetFirstString pou exei o client : " << printString;

        string command = printString;

        int server_fd;
        struct sockaddr_in server_out;
        struct hostent rem;

        if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }

        pthread_mutex_lock(&mvar);
        struct hostent * temprem = gethostbyname(servIP.c_str());
        /////////////
        // server_out.sin_addr.s_addr =inet_addr(servIP.c_str()) ;
        ////////////
        memcpy((void*) &rem, (void*) temprem, sizeof(rem));
        pthread_mutex_unlock(&mvar);

        server_out.sin_family = AF_INET; /* Internet domain */
        memcpy(&server_out.sin_addr, rem.h_addr, rem.h_length);
        server_out.sin_port = htons(servPort); /* Server port */

        if (connect(server_fd, (struct sockaddr*) &server_out, sizeof (server_out)) < 0) {
            perror("connect");
            exit(1);
        }

        Protocol protocol;

        string request[1] = {command};
        protocol.send(request, 1, server_fd);

        int size;
        string * response = protocol.receive(server_fd, &size);

        pthread_mutex_lock(&mvar);

        cout << "\n     *********************\n     request: " << command;
        for (int i = 0; i < size; i++) {
            cout << "     response " << i << " : " << response[i] << endl;
        }
        cout << "     *********************\n\n";
        pthread_mutex_unlock(&mvar);

        delete [] response;

        close(server_fd);
    }


    return 0;
};

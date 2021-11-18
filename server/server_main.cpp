#include <string>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

#include "Server.h"
#include "CountryInfo.h"
#include "Date.h"
#include "errors.h"
#include "Protocol.h"
#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main_worker(int argc, char *argv[]);

void * child(void * args);

void catchinterruptA1(int signo) { // grapse to log kai kane exit
    cout << "Aggregaotr Signo: " << signo << " catched (SIGINT/QUIT)" << endl;

    Server::shutdown_flag = true;
}

int main(int argc, char *argv[]) {
    ///whoServer -q queryPortNum -s statisticsPortNum -w numThreads -b bufferSize
    //./server -q 8888 -s 9999 -w 2 -b 4096
    // g++ -o server -pthread server_main.cpp Server.cpp Server.h CountryInfo.cpp CountryInfo.h Date.cpp Date.h 
    // errors.cpp errors.h Protocol.cpp Protocol.h Queue.cpp Queue.h ServerConfiguration.cpp ServerConfiguration.h
    
    int poolSize;
    int numWorkers;
    int statisticsPortNum;
    int queryPortNum;
    // int i;

    if (argc == 9) {

        int whichArg = 1;

        while (whichArg < 9) {
            if (strcmp(argv[whichArg], "-s") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    statisticsPortNum = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-q") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    queryPortNum = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-w") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    numWorkers = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-b") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    poolSize = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else {
                printf("Error : inputs are wrong , try/whoServer -q queryPortNum -s statisticsPortNum -w numThreads -b bufferSize\n");
                exit(-1);
            }
        }
    } else {
        // printArgumentError();
         printf("Error : inputs are wrong , try/whoServer -q queryPortNum -s statisticsPortNum -w numThreads -b bufferSize\n");
        exit(-1);
    }

    Server::queue = new Queue(poolSize);

    WorkerThread * workers = Server::setupCommunication(statisticsPortNum, queryPortNum, numWorkers, poolSize);

    printf("Server prepared!!! \n");

    static struct sigaction act;

    act.sa_handler = catchinterruptA1;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    Server::tids = new pthread_t[numWorkers];

    for (int i = 0; i < numWorkers; i++) {
        pthread_create(&Server::tids[i], NULL, child, NULL);
    }

    Server::serviceMaster(workers, numWorkers, poolSize);

    Server::serviceClients(workers, numWorkers, poolSize);

    Server::closeCommunication(numWorkers, workers);


    Server::shutdown(numWorkers);

    for (int i = 0; i < numWorkers; i++) {
        pthread_join(Server::tids[i], NULL);
    }

    CountryInfo::destroy();

    delete Server::queue;

    delete [] Server::tids;

    cout << "Aggregator exited with PID: " << getpid() << "\n";

    return 0;
}

void * child(void * args) {
    return Server::child();
}
#include <string>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

#include "AggregatorShell.h"
#include "CountryInfo.h"
#include "Date.h"
#include "errors.h"
#include "Protocol.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main_worker(int argc, char *argv[]);

void catchinterruptA1(int signo) { // grapse to log kai kane exit
    cout << "Aggregaotr Signo: " << signo << " catched (SIGINT/QUIT)" << endl;

}

int main(int argc, char *argv[]) {
    cout<<"3ekinhse o aggregator\n";

    // ./master -w numWorkers -b bufferSize -s serverIP  -p serverPort -i input_dir
    // ./aggregator -w 2 -b 4096 -s 127.0.0.1  -p 9999 -i input_dir
    //./aggregator -w 2 -b 4096 -s 192.168.1.10  -p 9999 -i input_dir 
    
    // g++ -o aggregator -pthread aggregator_main.cpp AggregatorShell.cpp AggregatorShell.h Country.cpp Country.h 
    // CountryInfo.cpp CountryInfo.h Date.cpp Date.h 
    // errors.cpp errors.h Protocol.cpp Protocol.h Worker.h worker_main.cpp WorkerShell.cpp WorkerShell.h Patient.cpp Patient.h
   

    // int bufferSize;
    int numWorkers;
    char* whichDirectory;
    char * server_ip;
    int server_port;
    
    if (argc == 11) {

        int whichArg = 1;

        while (whichArg < 11) {
            if (strcmp(argv[whichArg], "-i") == 0) {
                whichDirectory = argv[whichArg + 1];
                whichArg = whichArg + 2;
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
                    // bufferSize = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-s") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    server_ip = argv[whichArg + 1];
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else if (strcmp(argv[whichArg], "-p") == 0) {
                if (atoi(argv[whichArg + 1]) != 0) {
                    server_port = atoi(argv[whichArg + 1]);
                    whichArg = whichArg + 2;
                } else {
                    // whichArgumentError(argv[whichArg]);
                    exit(-1);
                }
            } else {
                printf("Error : inputs are wrong , try./[programName] -u [file].txt -w [integer] -b [integer]\n");
                exit(-1);
            }
        }
    } else {
        printf("arguments are wrong\n");
        // printArgumentError();
        exit(-1);
    }


    cout << "looking into: " << whichDirectory << endl;

    DIR * mydir = opendir(whichDirectory);

    if (mydir == NULL) {
        cout << "Directory: " << whichDirectory << " could not be opened " << endl;
        return -1;
    } else {
        struct dirent *direntp;
        int i = 0;

        while ((direntp = readdir(mydir)) != NULL) {
            if (isalpha(direntp->d_name[0])) {
                string name = direntp->d_name;
                CountryInfo::add(new CountryInfo(name, i % numWorkers));
                i++;
            }
        }
    }

    CountryInfo::print();

    Protocol::buffer = new char[Protocol::bufferSize];

    Worker * workers = AggregatorShell::setupCommunication(whichDirectory, numWorkers, Protocol::bufferSize, server_ip, server_port);

    printf("Aggregator prepared!!! \n");

    static struct sigaction act;

    act.sa_handler = catchinterruptA1;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    AggregatorShell::shell(workers, numWorkers, Protocol::bufferSize);

    // na grapsw ta log files!!!!!!!!!!!!
    AggregatorShell::closeCommunication(numWorkers, workers);



    CountryInfo::destroy();

    delete [] (char*) Protocol::buffer;
    
    
    

    cout << "Aggregator exited with PID: " << getpid() << "\n";

    return 0;
}
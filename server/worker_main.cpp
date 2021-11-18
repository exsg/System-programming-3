#include <string>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>

#include "WorkerShell.h"
#include "Country.h"
#include "Date.h"
#include "Protocol.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string global_inputdir;

void catchinterrupt1(int signo) { // write log + exit
    cout << "Signo: " << signo << " catched (SIGINT/QUIT)" << endl;
    WorkerShell::shutdown();
}

void catchinterrupt2(int signo) { // reload all files from disk
    cout << "Signo: " << signo << " catched (SIGUSR1)" << endl;
    WorkerShell::reload(global_inputdir);
    
    cout << "Signo: " << signo << " reload complete " << endl;
}

int main_worker(string inputdir, int w_to_a, int a_to_w) {
    int howmany = 0;
    
    cout << "Worker started with pid: " << getpid() << "\n";

    Protocol protocol;
    
    string * countries = protocol.receive(a_to_w, &howmany);

    for (int i = 0; i < howmany; i++) {
        Country::add(new Country(countries[i]));
        cout << countries[i];
    }


    Country::print();

    for (int i = 0; i < howmany; i++) {
        string whichDirectory = inputdir + "/" + countries[i];

        cout << "looking into: " << countries[i] << endl;

        DIR * mydir = opendir(whichDirectory.c_str());

        if (mydir == NULL) {
            cout << "Directory: " << whichDirectory << " could not be opened " << endl;
            return -1;
        } else {
            struct dirent *direntp;
            while ((direntp = readdir(mydir)) != NULL) {
                if (!isalpha(direntp->d_name[0])) {
                    string whichdate = direntp->d_name;
                    // cout << "loading records from: " << whichdate << endl;

                    string file = inputdir + "/" + countries[i] + "/" + whichdate;
                    Patient::readFile(countries[i], whichdate, file);
                }
            }
        }
    }

    //debugging --------------------------------
    //     string x;
    //     x="2045";
    //     Patient* p= WorkerShell::searchPatientRecord(x);
    //     cout<<p->id<<" "<<p->lastName<<endl;

    // Date date1;
    // date1.iDD = 12;
    // date1.iMM = 11;
    // date1.iYYYY = 1993;

    // Date date2;
    // date2.iDD = 12;
    // date2.iMM = 12;
    // date2.iYYYY = 1993;

    // if (compareDates(date1, date2)) {
    //     cout << "date1>date2\n";
    // } else {
    //     cout << "date1<date2\n";
    // }
    //debugging --------------------------------

    Patient::returnCompletePatient();

    cout << "howmany countries for this worker: " << howmany << endl;

    Date * lastDates = new Date[howmany];


    for (int i = 0; i < howmany; i++) {
        // string s = countries[i];
        lastDates[i] = Country::findLastDateForCountry(countries[i]);
        printf("latest date for %s is " , countries[i].c_str() );
        printDate(lastDates[i]);
    }

    global_inputdir = inputdir;
    
    static struct sigaction act;

    act.sa_handler = catchinterrupt1;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    act.sa_handler = catchinterrupt2;
    sigfillset(&(act.sa_mask));
    sigaction(SIGUSR1, &act, NULL);


    unsigned char reply = 1;

    printf("Worker prepared  !!! \n");

    ssize_t n = write(w_to_a, (char*) &reply, sizeof (unsigned char));


    if (n != sizeof (unsigned char)) {
        cout << "Worker: could not talk to aggregator, reply sent failed" << endl;
        exit(n);
    }

    WorkerShell::shell(w_to_a, a_to_w);

    ////////////////////////////////////////debuging
    // Date thisDate;
    // thisDate = Country::findLastDateForCountry(countries[0]);
    // printDate(thisDate);


    // Date thisDate2;
    // thisDate2 = Country::findLastDateForCountry(countries[1]);
    // printDate(thisDate2);


    // Date thisDate3;
    // thisDate3 = Country::findLastDateForCountry(countries[2]);
    // printDate(thisDate3);

    
    // cout<<"edw prepei na kanei print ton patient meta to shell\n";
    // thisp=Patient::returnPatientId("9819");
    // Patient::printPatient(thisp);
    // Patient::itteratePatientList(Patient::patientsHead);



    // Patient::itteratePatientList(Patient::completePatientList);
    // Patient::checkIfTwoTimes("6090");
    // Patient::checkIfTwoTimes("1980");
    // Patient::checkIfTwoTimes("5722");
    ////////////////////////////////////////////////////debuging
    Patient::destroy();


    Country::destroy();

    cout << "Worker exited with PID: " << getpid() << "\n";


    delete [] countries;



    // Patient::itteratePatientList( Patient::patientsHead);

    return 0;
}

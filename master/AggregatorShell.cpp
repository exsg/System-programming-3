#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "AggregatorShell.h"
#include "Date.h"
#include "Patient.h"
#include "Worker.h"
#include "Protocol.h"
#include "CountryInfo.h"
#include "WorkerShell.h"
#include "Country.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

Worker *AggregatorShell::workers = NULL;

bool AggregatorShell::shutdown_flag = false;

Patient *AggregatorShell::searchPatientRecord(string id) {
    
    return NULL;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void AggregatorShell::diseaseFrequency(string whatDisease, Date from, Date to, string country) {
    string *question = new string[5];
    question[0] = "diseaseFrequency";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country);

    if (selected == NULL) {
        printf("AggregatorShell:: diseaseFrequency error,  Country does not exist\n");
        return;
    }

    Protocol::send(question, 5, selected->a_to_w);

    int size = 0;

    string *answer = Protocol::receive(selected->w_to_a, &size);

    cout << "------------------ Aggregator ------------------- " << endl;
    for (int i = 0; i < size; i++) {
        cout << answer[i] << endl;
    }

    delete[] question;
    delete[] answer;
}
//isws xreiazetai na pernaw ton ari8mo twn workers me pointer

void AggregatorShell::diseaseFrequency(string whatDisease, Date from, Date to, int numWorkers) {

    string *question = new string[4];
    question[0] = "diseaseFrequency";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    // Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country);
    // int howManycountries = CountryInfo::countAllCountries();
    for (int i = 0; i < numWorkers; i++) {
        // int workerCountriesCount = CountryInfo::countCountriesForWorker(i);
        string *workerCountriesTable = CountryInfo::getCountries(i);

        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, workerCountriesTable[0]);
        Protocol::send(question, 4, selected->a_to_w);
    }

    string *answer;
    int diseaseFrequencySum = 0;
    for (int i = 0; i < numWorkers; i++) {
        // int workerCountriesCount = CountryInfo::countCountriesForWorker(i);
        string *workerCountriesTable = CountryInfo::getCountries(i);

        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, workerCountriesTable[0]);
        int size = 0;
        answer = Protocol::receive(selected->w_to_a, &size);
        for (int j = 0; j < size; j++) {
            // cout << answer[j] << endl;
            diseaseFrequencySum = diseaseFrequencySum + atoi(answer[j].c_str());
        }
    }

    cout << "AggregatorShell::diseaseFrequency without country :" << diseaseFrequencySum << " total cases of " << whatDisease << endl;

    delete[] question;
    delete[] answer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void AggregatorShell::numPatientAdmissions(string whatDisease, Date from, Date to, string country) {

    cout << "mphke sto numPatientAdmissions ME xwra\n";
    string *question = new string[5];
    question[0] = "numPatientAdmissions";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country);

    if (selected == NULL) {
        printf("AggregatorShell:: numPatientAdmissions error,  Country does not exist\n");
        return;
    }

    Protocol::send(question, 5, selected->a_to_w);

    int size = 0;

    string *answer = Protocol::receive(selected->w_to_a, &size);

    cout << "------------------ Aggregator ------------------- " << endl;
    for (int i = 0; i < size; i++) {
        cout << answer[i] << endl;
    }

    delete[] question;
    delete[] answer;
}

void AggregatorShell::numPatientAdmissions(string whatDisease, Date from, Date to, int numWorkers) {

  
    string *question = new string[5];
    question[0] = "numPatientAdmissions";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    CountryInfo *country = CountryInfo::countriesHead;

    while (country != NULL) {
        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country->name);
        question[4] = country->name;
        Protocol::send(question, 5, selected->a_to_w);

        country = country->nextCountryInfo;
    }
    cout << "------------------ Aggregator ------------------- " << endl;

    string *answer;
    country = CountryInfo::countriesHead;
    while (country != NULL) {
        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country->name);
        int size = 0;
        answer = Protocol::receive(selected->w_to_a, &size);
        cout << country->name << " ";
        for (int i = 0; i < size; i++) {
            cout << answer[i] << endl;
        }
        country = country->nextCountryInfo;
    }


    delete[] question;
    delete[] answer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void AggregatorShell::numPatientDischarges(string whatDisease, Date from, Date to, string country) {
    string *question = new string[5];
    question[0] = "numPatientDischarges";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country);

    if (selected == NULL) {
        printf("AggregatorShell:: numPatientDischarges error,  Country does not exist\n");
        return;
    }

    Protocol::send(question, 5, selected->a_to_w);

    int size = 0;

    string *answer = Protocol::receive(selected->w_to_a, &size);

    cout << "------------------ Aggregator ------------------- " << endl;
    for (int i = 0; i < size; i++) {
        cout << answer[i] << endl;
    }

    delete[] question;
    delete[] answer;
}

void AggregatorShell::numPatientDischarges(string whatDisease, Date from, Date to, int numWorkers) {

 

    string *question = new string[5];
    question[0] = "numPatientDischarges";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    CountryInfo *country = CountryInfo::countriesHead;

    while (country != NULL) {
        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country->name);
        question[4] = country->name;
        Protocol::send(question, 5, selected->a_to_w);

        country = country->nextCountryInfo;
    }
    cout << "------------------ Aggregator ------------------- " << endl;

    string *answer;
    country = CountryInfo::countriesHead;
    while (country != NULL) {
        Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country->name);
        int size = 0;
        answer = Protocol::receive(selected->w_to_a, &size);
        cout << country->name << " ";
        for (int i = 0; i < size; i++) {
            cout << answer[i] << endl;
        }
        country = country->nextCountryInfo;
    }


    delete[] question;
    delete[] answer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void AggregatorShell::topkAgeRanges(int k, string whatDisease, Date from, Date to, string country) {
    string *question = new string[6];
    question[0] = "topk-AgeRanges";
    question[1] = to_string(k);
    question[2] = country;
    question[3] = whatDisease;
    question[4] = from.date;
    question[5] = to.date;


    Worker *selected = CountryInfo::getWorker(AggregatorShell::workers, country);
    if (selected == NULL) {
        printf("AggregatorShell:: topkAgeRanges error,  Country does not exist\n");
        return;
    }

    Protocol::send(question, 6, selected->a_to_w);

    int size = 0;

    string *answer = Protocol::receive(selected->w_to_a, &size);

    cout << "------------------ Aggregator ------------------- " << endl;
    for (int i = 0; i < size; i++) {
        cout << answer[i] << endl;
    }

    delete[] question;
    delete[] answer;


}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void AggregatorShell::shutdown() {
    cout << "AggregatorShell::shutdown " << endl;
    AggregatorShell::shutdown_flag = true;
}


void AggregatorShell::shell(Worker *workers, int numWorkers, int bufferSize) {
    char line[2000];

    AggregatorShell::workers = workers;

    sigset_t set1;

    sigemptyset(&set1);

    sigaddset(&set1, SIGINT);
    sigaddset(&set1, SIGQUIT);

    while (AggregatorShell::shutdown_flag == false) {
        cout << "command? ";

        sigprocmask(SIG_UNBLOCK, &set1, NULL);

        if (AggregatorShell::shutdown_flag == true) {
            break;
        }
        
        char *ttttt = fgets(line, sizeof (line), stdin);
        
        if (AggregatorShell::shutdown_flag == true) {
            break;
        }

        sigprocmask(SIG_SETMASK, &set1, NULL);

        if (ttttt == NULL) {
            return;
        }

        if (line[0] != '/') {
            string s = line;
            string w = "/" + s;
            strcpy(line, w.c_str());
        }

        char *c1 = NULL;
        char *c2 = NULL;
        char *c3 = NULL;
        char *c4 = NULL;
        char *c5 = NULL;
        char *c6 = NULL;
        char *c7 = NULL;
        char *c8 = NULL;

        int c = sscanf(line, "%ms %ms %ms %ms %ms %ms %ms %ms", &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8);

        if (c1 == NULL) {
            printf("No input at all , please type a command OR type: exit.\n");
            continue;
        }
        //mhpws prepei na ta kanw free?
        if (strcmp(c1, "/exit") == 0) {
            free(c1);
            break;
        }

        if (strcmp(c1, "/listCountries") == 0) {
            CountryInfo::print();
        }

        if (strcmp(c1, "/diseaseFrequency") == 0) {
            // printf("num current Disease\n");

            if (c == 4) { // without country

                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                diseaseFrequency(whatDisease, from, to, numWorkers);
            } else if (c == 5) { //WITH  COUNTRY
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                diseaseFrequency(whatDisease, from, to, whatCountry);
            } else {
                printf("Error : diseaseFrequency needs Virus Date1 Date2 OR Virus Country Date1 Date2\n");
            }
        } else if (strcmp(c1, "/topk-AgeRanges") == 0) {
            if (c == 6) {
                printf("myshell : /topK-AgeRanges\n");
                int k = atoi(c2);
                string whatCountry = (c3);
                string whatDisease = (c4);
                Date from = Date(c5);
                Date to = Date(c6);
                // printf("%d \n" , k);
                cout << whatCountry << " " << whatDisease << endl;
                topkAgeRanges(k, whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /topKAgeRanges needs to be 5 and it is %d\n", (int) c);
            }
        } else if (strcmp(c1, "/searchPatientRecord") == 0) {
            if (c == 2) {
                printf("myshell : /searchPatientRecord\n");
            } else {
                printf("myshell : error in the arguments for/searchPatientRecord needs to be 2 and it is %d\n", (int) c);
            }
        } else if (strcmp(c1, "/numPatientDischarges") == 0) {
            if (c == 4) {
                printf("myshell :  /numPatientDischarges WITHOUT country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                // string whatCountry(c5);
                numPatientDischarges(whatDisease, from, to, numWorkers);
            } else if (c == 5) {
                printf("myshell :  /numPatientDischarges WITH country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientDischarges(whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /numPatientDischarges\n");
            }
        } else if (strcmp(c1, "/numPatientAdmissions") == 0) {
            if (c == 4) {
                printf("myshell :  /numPatientAdmissions WITHOUT country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                // string whatCountry(c5);
                numPatientAdmissions(whatDisease, from, to, numWorkers);
            } else if (c == 5) {
                printf("myshell :  /numPatientAdmissions WITH country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientAdmissions(whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /numPatientAdmissions\n");
            }
        }

        // -------------------------------------------------------------------

        if (c1 != NULL) {
            free(c1);
        }
        if (c2 != NULL) {
            free(c2);
        }
        if (c3 != NULL) {
            free(c3);
        }
        if (c4 != NULL) {
            free(c4);
        }
        if (c5 != NULL) {
            free(c5);
        }
        if (c6 != NULL) {
            free(c6);
        }
        if (c7 != NULL) {
            free(c7);
        }
        if (c8 != NULL) {
            free(c8);
        }
    }
}

Worker *AggregatorShell::setupCommunication(string whichDirectory, int numWorkers, int bufferSize, string server_ip , int server_port) {
    Worker *workers = new Worker[numWorkers];

    mkdir("pipes", 0755);

    for (int i = 0; i < numWorkers; i++) {
        workers[i].id = i;
        sprintf(workers[i].w_to_a_name, "pipes/w_to_a_%d.pipe", workers[i].id);
        sprintf(workers[i].a_to_w_name, "pipes/a_to_w_%d.pipe", workers[i].id);

        if (mkfifo(workers[i].w_to_a_name, 0666) == -1) {
            printf("warning mkfifo (not exit) \n");
        }

        if (mkfifo(workers[i].a_to_w_name, 0666) == -1) {
            printf("warning mkfifo (not exit) \n");
        }

        workers[i].pid = fork();

        if (workers[i].pid > 0) {
            workers[i].w_to_a = open(workers[i].w_to_a_name, O_RDONLY);
            workers[i].a_to_w = open(workers[i].a_to_w_name, O_WRONLY);

            int howmany = CountryInfo::countCountriesForWorker(i);

            string *countries = CountryInfo::getCountries(i);

            Protocol::send(countries, howmany, workers[i].a_to_w);

            delete[] countries;
        }
        
        string connectiondata[2];
        connectiondata[0] = server_ip;
        connectiondata[1] = to_string(server_port);
        
        Protocol::send(connectiondata, 2, workers[i].a_to_w);

        ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

        if (workers[i].pid == 0) {
            int w_to_a = open(workers[i].w_to_a_name, O_WRONLY);
            int a_to_w = open(workers[i].a_to_w_name, O_RDONLY);

            printf("Worker stared: w_to_a:%d a_to_w:%d \n", w_to_a, a_to_w);

            main_worker(whichDirectory, w_to_a, a_to_w, numWorkers);

            close(w_to_a);
            close(a_to_w);

            exit(0);
        }

        ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    }

    unsigned char replies = 0;
    unsigned char reply;

    for (int i = 0; i < numWorkers; i++) {
        ssize_t n = read(workers[i].w_to_a, (char *) &reply, sizeof (unsigned char));

        if (n != sizeof (unsigned char)) {
            cout << "A worker has not initialized properly, read failed for reply token" << endl;
            cout << "n = " << n << endl;
            exit(n);
        }

        replies = replies + reply;
    }

    if (replies != numWorkers) {
        cout << "A worker has not initialized properly. replies != numWorkers " << endl;
        exit(replies);
    }

    return workers;
}

void AggregatorShell::closeCommunication(int numWorkers, Worker *workers) {
    for (int i = 0; i < numWorkers; i++) {
        kill(workers[i].pid, SIGKILL);
        wait(NULL);
    }

    for (int i = 0; i < numWorkers; i++) {
        close(workers[i].a_to_w);
        close(workers[i].w_to_a);

        unlink(workers[i].a_to_w_name);
        unlink(workers[i].w_to_a_name);
    }
}
#include <string>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include <signal.h>

using namespace std;

#include "WorkerShell.h"
#include "Date.h"
#include "Patient.h"
#include "Protocol.h"
#include "Country.h"

#include <stdio.h>
#include <stdlib.h>

class AgeRangeFreq {
public:
    int ageFrom;
    int ageTo;
    int frequency;

    AgeRangeFreq() {
        ageFrom = 0;
        ageTo = 0;
        frequency = 0;
    };

};

int WorkerShell::w_to_a = 0;
int WorkerShell::a_to_w = 0;

void WorkerShell::insertPatientRecord(Patient *pn) {
}

Patient *WorkerShell::searchPatientRecord(string id) {
    if (Patient::patientsHead != NULL) {
        Patient *temp = Patient::patientsHead;
        while (temp->nextPatient != NULL) {
            if (string(temp->id) == id) {

                return temp;
            }
            temp = temp->nextPatient;
        }
        if (string(temp->id) == id) {
            return temp;
        } else {
            cout << "no patient with id %d found" << id << endl;
            return NULL;
        }
    } else {
        cout << "Patient List is empty \n";
        return NULL; //prosoxh , gurizw NULL an einai empty opote prepei na to exw sto noy moy
    }
}
///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void WorkerShell::diseaseFrequency(string whatDisease, Date from, Date to, string country) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: diseaseFrequency error,  List is empty\n");
    } else {
        Patient *p;
        // p = Patient::patientsHead;
        p = Patient::completePatientList;

        while (p->nextPatient != NULL) {
            if ((p->disease == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (p->country == country)) {
                counter++;
            } else {



                // printf("diseaseFrequencyReport  for %d-%d-%d to %d-%d-%d , value: %d-%d-%d \n", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY,p->entryDate.iDD, p->entryDate.iMM, p->entryDate.iYYYY );
                //                cout << (p->disease == whatDisease) << " " <<
                //                        compareDates(p->entryDate, from) << " " <<
                //                        compareDates(to, p->entryDate) << " " <<
                //                        (p->country == country) << endl;
            }
            p = p->nextPatient;
        }
        if ((p->disease == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (p->country == country)) {
            counter++;
        }
    }
    printf("WorkerShell:: diseaseFrequencyReport  for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    cout << " for country " << country << " and for disease " << whatDisease;
    printf(" frequency :%d \n", counter);

    Protocol protocol;
    
    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);

    protocol.send(answer, 1, WorkerShell::w_to_a);

    delete [] answer;
}

void WorkerShell::diseaseFrequency(string whatDisease, Date from, Date to) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: diseaseFrequency error,  List is empty\n");
    } else {
        Patient *p;
        // p = Patient::patientsHead;
        p = Patient::completePatientList;

        while (p->nextPatient != NULL) {
            if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate))) {
                counter++;
            }
            p = p->nextPatient;
        }
        if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate))) {
            counter++;
        }
    }
    // cout<<"diseaseFrequencyReport WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d "<<from.iDD , from.iMM , from.iYYYY , to.iDD,to.iMM,to.iYYYY;
    printf("WorkerShell:: diseaseFrequencyReport WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    cout << "and for disease " << whatDisease;
    printf("frequency = %d\n", counter);
    Protocol protocol;
    
    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);

    protocol.send(answer, 1, WorkerShell::w_to_a);
    delete [] answer;
}
///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void WorkerShell::numPatientAdmissions(string whatDisease, Date from, Date to, string country) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: numPatientAdmissions error,  List is empty\n");
    } else {
        Patient *p;
        p = Patient::completePatientList;
        // p = Patient::patientsHead;

        while (p->nextPatient != NULL) {
            // Patient::printPatient(p);
            if ((p->disease == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (p->country == country)) {
                counter++;
            }
            p = p->nextPatient;
        }
        if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (string(p->country) == country)) {
            counter++;
        }
    }
    // printf("WorkerShell:: numPatientAdmissions  for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    // cout << " for country " << country << " and for disease " << whatDisease;
    // printf(" Admissions :%d \n", counter);

    Protocol protocol;
    
    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);

    protocol.send(answer, 1, WorkerShell::w_to_a);

    delete [] answer;
}

void WorkerShell::numPatientAdmissions(string whatDisease, Date from, Date to) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: numPatientAdmissions error,  List is empty\n");
    } else {
        Patient *p;
        p = Patient::completePatientList;

        while (p->nextPatient != NULL) {
            if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate))) {
                counter++;
            }
            p = p->nextPatient;
        }
        if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate))) {
            counter++;
        }
    }
    // cout<<"diseaseFrequencyReport WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d "<<from.iDD , from.iMM , from.iYYYY , to.iDD,to.iMM,to.iYYYY;
    // printf("WorkerShell:: numPatientAdmissions WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    // cout << "and for disease " << whatDisease;
    // printf(" Admissions = %d\n", counter);

    Protocol protocol;
    
    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);

    protocol.send(answer, 1, WorkerShell::w_to_a);
    delete [] answer;
}
///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void WorkerShell::numPatientDischarges(string whatDisease, Date from, Date to, string country) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: numPatientDischarges error,  List is empty\n");
    } else {
        Patient *p;
        p = Patient::completePatientList;

        while (p->nextPatient != NULL) {
            if ((p->disease == whatDisease) && (compareDates(p->exitDate, from)) && (compareDates(to, p->exitDate)) && (p->country == country) && (p->exitDate.iDD > 0)) {
                counter++;
            }
            p = p->nextPatient;
        }
        if ((p->disease == whatDisease) && (compareDates(p->exitDate, from)) && (compareDates(to, p->exitDate)) && (p->country == country) && (p->exitDate.iDD > 0)) {
            counter++;
        }
    }

    // printf("WorkerShell:: numPatientDischarges  for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    // cout << " for country " << country << " and for disease " << whatDisease;
    // printf("Discharges :%d \n", counter);

    Protocol protocol;

    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);
    protocol.send(answer, 1, WorkerShell::w_to_a);

    delete [] answer;
}

void WorkerShell::numPatientDischarges(string whatDisease, Date from, Date to) {
    int counter = 0;
    if (Patient::completePatientList == NULL) {
        printf("WorkerShell:: numPatientDischarges error,  List is empty\n");
    } else {
        Patient *p;
        p = Patient::completePatientList;

        while (p != NULL) {
            if ((p->disease == whatDisease) && (compareDates(p->exitDate, from)) && (compareDates(to, p->exitDate)) && (p->exitDate.iDD > 0)) {
                counter++;
            }
            p = p->nextPatient;
        }
        // if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (p->exitDate.iDD > 0)) {
        //     counter++;
        // }
    }
    // cout<<"diseaseFrequencyReport WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d "<<from.iDD , from.iMM , from.iYYYY , to.iDD,to.iMM,to.iYYYY;
    // printf("numPatientDischarges WITHOUT COUNTRY for %d-%d-%d to %d-%d-%d ", from.iDD, from.iMM, from.iYYYY, to.iDD, to.iMM, to.iYYYY);
    // cout << "and for disease " << whatDisease;
    // printf("Discharges = %d\n", counter);

    Protocol protocol;

    string * answer = new string[1];
    answer[0] = protocol.to_string(counter);


    protocol.send(answer, 1, WorkerShell::w_to_a);
    delete [] answer;
}

void WorkerShell::topkAgeRanges(int k, string whatDisease, Date from, Date to, string country) {
    //4 age groups
    AgeRangeFreq group[4];
    group[0].ageFrom = 0;
    group[0].ageTo = 20;
    group[0].frequency = 0;

    group[1].ageFrom = 21;
    group[1].ageTo = 40;
    group[1].frequency = 0;

    group[2].ageFrom = 41;
    group[2].ageTo = 60;
    group[2].frequency = 0;

    group[3].ageFrom = 61;
    group[3].ageTo = 120;
    group[3].frequency = 0;

    int counter[4] = {0, 0, 0, 0};
    int everyCase = 0;
    if (Patient::completePatientList != NULL) {
        Patient *p = Patient::completePatientList;
        while (p != NULL) {
            if ((string(p->disease) == whatDisease) && (compareDates(p->entryDate, from)) && (compareDates(to, p->entryDate)) && (string(p->country) == country)) {
                if ((p->age >= 0) && (p->age <= 20)) {
                    counter[0]++;
                    group[0].frequency++;
                    everyCase++;
                } else if ((p->age >= 21) && (p->age <= 40)) {
                    counter[1]++;
                    group[1].frequency++;
                    everyCase++;
                } else if ((p->age >= 41) && (p->age <= 60)) {
                    counter[2]++;
                    group[2].frequency++;
                    everyCase++;
                } else if (p->age >= 61) {
                    counter[3]++;
                    group[3].frequency++;
                    everyCase++;
                } else {
                    cout << "age<0 error";
                }

            }
            p = p->nextPatient;
        }

    } else {
        cout << "list is empty\n";
        return;
    }
    // printf("meta th while counter= %d %d %d %d\n",counter[0],counter[1],counter[2], counter[3]);
    //bubblesort
    for (int i = 0; i <= 2; i++) {

        for (int j = i + 1; j <= 3; j++) {
            //            int x;
            AgeRangeFreq x1;
            // if (counter[j] > counter[i])
            // {
            //     printf("counter[i]=%d counter[j]=%d\n", counter[i], counter[j]);
            //     x = counter[i];
            //     counter[i] = counter[j];
            //     counter[j] = x;
            // }
            if (group[j].frequency > group[i].frequency) {
                x1 = group[i];
                group[i] = group[j];
                group[j] = x1;
            }
        }
    }
    string * answer = new string[k];
    // answer[0] ="Age range "+to_string(group[i].ageFrom)+"-"+to_string(group[i].ageTo)+" years :"+to_string(group[i].frequency)+" cases "+"("+to_string(100*((float)group[i].frequency)/((float)everyCase)+"%)");




    if (k > 0) {
        if (k > 4) {
            cout << "given Age groups exceed the groups that exist , %d is transformed to 4\n";
            k = 4;
        }
        for (int i = 0; i <= k - 1; i++) {
            printf("Age range %d-%d years: %d cases %.2f %% of total\n", group[i].ageFrom, group[i].ageTo, group[i].frequency, 100 * ((float) group[i].frequency) / ((float) everyCase));
            // Age range 0-20 years: 10 cases
            answer[i] = "Age range " + to_string(group[i].ageFrom) + "-" + to_string(group[i].ageTo) + " years :" + to_string(group[i].frequency) + " cases " + to_string(100 * ((float) group[i].frequency) / ((float) everyCase)) + "%";

        }
    } else {
        cout << "please give more than 0 for k\n";
    }

    Protocol protocol;
    protocol.send(answer, k, WorkerShell::w_to_a);

    delete [] answer;


}

void WorkerShell::reload(string inputdir) {
    cout << "WorkerShell::reload " << endl;

    // TODO: reload files

    int howmany = 0;
    Country *temp = Country::countriesHead;

    while (temp != NULL) {
        howmany++;
        temp = temp->nextCountry;
    }


    temp = Country::countriesHead;

    for (int i = 0; i < howmany; i++) {
        string country = temp->name;

        string whichDirectory = inputdir + "/" + country;

        cout << "looking into: " << country << endl;

        DIR * mydir = opendir(whichDirectory.c_str());

        if (mydir == NULL) {
            cout << "Directory: " << whichDirectory << " could not be opened " << endl;
            return;
        } else {
            struct dirent *direntp;
            while ((direntp = readdir(mydir)) != NULL) {
                if (!isalpha(direntp->d_name[0])) {
                    string whichdate = direntp->d_name;
                    cout << "reloading records from: " << whichdate << endl;

                    string file = inputdir + "/" + country + "/" + whichdate;
                    Patient::readFile(country, whichdate, file);
                }
            }
        }

        temp = temp->nextCountry;
    }


}

void WorkerShell::shutdown() {
    cout << "WorkerShell::shutdown " << endl;

    // TODO: write log:

    // 1) all countries

    // 2) success and fail counters with total


    exit(0);
}

void WorkerShell::shell(int w_to_a, int a_to_w) {
    char line[2000];

    WorkerShell::w_to_a = w_to_a;
    WorkerShell::a_to_w = a_to_w;

    sigset_t set1;

    sigemptyset(&set1);

    sigaddset(&set1, SIGINT);
    sigaddset(&set1, SIGQUIT);
    sigaddset(&set1, SIGUSR1);


    while (1) {
        cout << "command? ";

        Protocol protocol;

        int size = 0;

        sigprocmask(SIG_UNBLOCK, &set1, NULL);

        string * question = protocol.receive(WorkerShell::a_to_w, & size);

        sigprocmask(SIG_SETMASK, &set1, NULL);

        strcpy(line, "");

        for (int i = 0; i < size; i++) {
            strcat(line, question[i].c_str());
            if (i < size - 1) {
                strcat(line, " ");
            }
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

        if (strcmp(c1, "/insertPatientRecord") == 0) {
            if (c == 7) { // record without date
                string id(c2);
                string firstName(c3);
                string lastName(c4);
                string whatDisease(c5);
                string whatCountry(c6);
                Date entryDate(c7);
                Date exitDate(c8);


                //                }
            } else if (c == 8) { // record with date

            } else { // error
                puts("Error");
            }
        } else if (strcmp(c1, "/recordPatientExit") == 0) {

        } else if (strcmp(c1, "/diseaseFrequency") == 0) {
            // printf("num current Disease\n");

            if (c == 4) { // without country

                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                diseaseFrequency(whatDisease, from, to);
            } else if (c == 5) { //WITH  COUNTRY
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                diseaseFrequency(whatDisease, from, to, whatCountry);
            } else {
                printf("Error : diseaseFrequency needs Virus Date1 Date2 OR Virus Country Date1 Date2\n");
            }
        } else if (strcmp(c1, "/numPatientAdmissions") == 0) {
            if (c == 4) {
                // printf("myshell :  /numPatientAdmissions WITHOUT country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);

                numPatientAdmissions(whatDisease, from, to);
            } else if (c == 5) {
                // printf("myshell :  /numPatientAdmissions WITH country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientAdmissions(whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /numPatientAdmissions\n");
            }
        } else if (strcmp(c1, "/topk-AgeRanges") == 0) {
            if (c == 6) {
                // printf("WorkerShell : /topK-AgeRanges\n");
                int k = atoi(c2);
                string whatCountry = (c3);
                string whatDisease = (c4);
                Date from = Date(c5);
                Date to = Date(c6);
                // printf("%d \n" , k);
                cout << whatCountry << " " << whatDisease << endl;
                topkAgeRanges(k, whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /topKAgeRanges needs to be 5 and it is %d\n", (int) (c - 1));
            }
        } else if (strcmp(c1, "/searchPatientRecord") == 0) {
            if (c == 2) {
                printf("myshell : /searchPatientRecord\n");
            } else {
                printf("myshell : error in the arguments for/searchPatientRecord needs to be 2 and it is %d\n", (int) (c - 1));
            }
        } else if (strcmp(c1, "/numPatientDischarges") == 0) {
            if (c == 4) {
                // printf("myshell :  /numPatientDischarges WITHOUT country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                // string whatCountry(c5);
                numPatientDischarges(whatDisease, from, to);
            } else if (c == 5) {
                // printf("myshell :  /numPatientDischarges WITH country\n");
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientDischarges(whatDisease, from, to, whatCountry);
            } else {
                printf("myshell : error in the arguments for /numPatientDischarges\n");
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

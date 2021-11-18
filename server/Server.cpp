#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>      /* sockets */
#include <sys/socket.h>      /* sockets */
#include <netinet/in.h>      /* internet sockets */
#include <arpa/inet.h>
#include <netdb.h>

#include "Server.h"
#include "Date.h"
#include "Patient.h"
#include "WorkerThread.h"
#include "Protocol.h"
#include "CountryInfo.h"
#include "WorkerShell.h"
#include "Country.h"
#include "ServerConfiguration.h"

#include "MasterWorker.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

WorkerThread *Server::workerThreads = NULL;

MasterWorker* Server::masterWorkers = NULL;

pthread_t * Server::tids = NULL;
Queue * Server::queue = NULL;
bool Server::shutdown_flag = false;

pthread_mutex_t Server::mvar = PTHREAD_MUTEX_INITIALIZER;

Patient *Server::searchPatientRecord(string id) {

    return NULL;
}

int * Server::connectToMasterWorkers() {
    int numWorkers = Server::masterWorkers[0].how_many_workers;
    int * fds = new int[numWorkers];

    for (int i = 0; i < numWorkers; i++) {
        struct sockaddr_in server_out;
        struct hostent rem;

        if ((fds[i] = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }

        pthread_mutex_lock(&Server::mvar);
        struct hostent * temprem = gethostbyname(Server::masterWorkers[i].ip.c_str());
        memcpy((void*) &rem, (void*) temprem, sizeof (rem));
        pthread_mutex_unlock(&Server::mvar);


        server_out.sin_family = AF_INET; /* Internet domain */
        memcpy(&server_out.sin_addr, rem.h_addr, rem.h_length);
        server_out.sin_port = htons(Server::masterWorkers[i].port); /* Server port */

        if (connect(fds[i], (struct sockaddr*) &server_out, sizeof (server_out)) < 0) {
            perror("connect");
            exit(1);
        }

    }

    return fds;
}

int * Server::connectToMasterWorkers(string country) {
    int numWorkers = Server::masterWorkers[0].how_many_workers;
    int * fds = new int[1];

    for (int i = 0; i < numWorkers; i++) {
        for (int j = 0; j < Server::masterWorkers[i].how_many_countries; j++) {
            if (Server::masterWorkers[i].countries[j] == country) {
                // printf("vre8hke o worker upeu8unos gia th xwra index=%d country=%s\n" , i  ,country.c_str());
                struct sockaddr_in server_out;
                struct hostent rem;

                if ((fds[0] = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                    perror("################# socket");
                    exit(1);
                }

                pthread_mutex_lock(&Server::mvar);
                struct hostent * temprem = gethostbyname(Server::masterWorkers[i].ip.c_str());
                memcpy((void*) &rem, (void*) temprem, sizeof (rem));
                pthread_mutex_unlock(&Server::mvar);


                server_out.sin_family = AF_INET; /* Internet domain */
                memcpy(&server_out.sin_addr, rem.h_addr, rem.h_length);
                server_out.sin_port = htons(Server::masterWorkers[i].port); /* Server port */

                if (connect(fds[0], (struct sockaddr*) &server_out, sizeof (server_out)) < 0) {
                    perror("################# connect");
                    exit(1);
                }

                return fds;
            }
           
            //else return NULL and see from the other side what to do alliws trww segmentation fault
        }
    }

    return NULL;
}
///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void Server::diseaseFrequency(int reply_fd, string whatDisease, Date from, Date to, string country) {
    Protocol protocol;
    string *question = new string[5];
    question[0] = "diseaseFrequency";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    int numWorkers = 1;
    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers(country);

    if(s_to_w==NULL){
        printf("o server den 8a gurisei tpt giati den yparxei tetoia xwra\n");
        return;
    }
    
    printf("Server::diseaseFrequency Report \nFile descriptor for country %s = %d\n\n", country.c_str() , *s_to_w);

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 5, s_to_w[i]);
    }


    int diseaseFrequencySum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        diseaseFrequencySum += atoi(answers[i][0].c_str());
        
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::diseaseFrequency without country :" << diseaseFrequencySum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(diseaseFrequencySum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;
}

void Server::diseaseFrequency(int reply_fd, string whatDisease, Date from, Date to, int numWorkers) {
    Protocol protocol;

    string *question = new string[4];
    question[0] = "diseaseFrequency";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers();

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 4, s_to_w[i]);
    }


    int diseaseFrequencySum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        diseaseFrequencySum += atoi(answers[i][0].c_str());
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::diseaseFrequency without country :" << diseaseFrequencySum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(diseaseFrequencySum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void Server::numPatientAdmissions(int reply_fd, string whatDisease, Date from, Date to, string country) {

    Protocol protocol;
    string *question = new string[5];
    question[0] = "numPatientAdmissions";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    int numWorkers = 1;
    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers(country);

    if(s_to_w==NULL){
        printf("o server den 8a gurisei tpt giati den yparxei tetoia xwra\n");
        return;
    }
    
    printf("Server::numPatientAdmissions Report \nFile descriptor for country %s = %d\n\n", country.c_str() , *s_to_w);

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 5, s_to_w[i]);
    }


    int numPatientAdmissionsSum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        numPatientAdmissionsSum += atoi(answers[i][0].c_str());
        
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::diseaseFrequency without country :" << numPatientAdmissionsSum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(numPatientAdmissionsSum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;

    
}

void Server::numPatientAdmissions(int reply_fd, string whatDisease, Date from, Date to, int numWorkers) {


   
    Protocol protocol;

    string *question = new string[4];
    question[0] = "numPatientAdmissions";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers();

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 4, s_to_w[i]);
    }


    int numPatientAdmissionsSum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        numPatientAdmissionsSum += atoi(answers[i][0].c_str());
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::numPatientAdmissions without country :" << numPatientAdmissionsSum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(numPatientAdmissionsSum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void Server::numPatientDischarges(int reply_fd, string whatDisease, Date from, Date to, string country) {
    Protocol protocol;
    string *question = new string[5];
    question[0] = "numPatientDischarges";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;
    question[4] = country;

    int numWorkers = 1;
    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers(country);

    if(s_to_w==NULL){
        printf("o server den 8a gurisei tpt giati den yparxei tetoia xwra\n");
        return;
    }
    
    printf("Server::numPatientDischarges Report \nFile descriptor for country %s = %d\n\n", country.c_str() , *s_to_w);

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 5, s_to_w[i]);
    }


    int numPatientDischargesSum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        numPatientDischargesSum += atoi(answers[i][0].c_str());
        
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::diseaseFrequency without country :" << numPatientDischargesSum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(numPatientDischargesSum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;

    
}

void Server::numPatientDischarges(int reply_fd, string whatDisease, Date from, Date to, int numWorkers) {

   
    Protocol protocol;

    string *question = new string[4];
    question[0] = "numPatientDischarges";
    question[1] = whatDisease;
    question[2] = from.date;
    question[3] = to.date;

    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers();

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 4, s_to_w[i]);
    }


    int numPatientDischargesSum = 0;

    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }

    for (int i = 0; i < numWorkers; i++) {
        numPatientDischargesSum += atoi(answers[i][0].c_str());
    }

    for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }

    pthread_mutex_lock(&Server::mvar);
    cout << "Server::numPatientDischarges without country :" << numPatientDischargesSum << " total cases of " << whatDisease << endl;
    pthread_mutex_unlock(&Server::mvar);


    biganswer = new string[1];

    biganswer[0] = to_string(numPatientDischargesSum);

    protocol.send(biganswer, 1, reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }


    delete[] question;
    delete[] biganswer;
}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void Server::topkAgeRanges(int reply_fd, int k, string whatDisease, Date from, Date to, string country) {

     Protocol protocol;
    string *question = new string[6];
    question[0] = "topk-AgeRanges";
    question[1] = to_string(k);
    question[2] = country;
    question[3] = whatDisease;
    question[4] = from.date;
    question[5] = to.date;

    int numWorkers = 1;
    string * answers[numWorkers];
    int sizes[numWorkers];
    string *biganswer;
    int * s_to_w = Server::connectToMasterWorkers(country);

    if(s_to_w==NULL){
        printf("o server den 8a gurisei tpt giati den yparxei tetoia xwra\n");
        return;
    }
    
    printf("Server::topk-AgeRanges Report \nFile descriptor for country %s = %d\n\n", country.c_str() , *s_to_w);

    for (int i = 0; i < numWorkers; i++) {
        protocol.send(question, 6, s_to_w[i]);
    }


    for (int i = 0; i < numWorkers; i++) {
        answers[i] = protocol.receive(s_to_w[i], &sizes[i]);
    }


    biganswer = new string[sizes[0]];
    for (int i = 0; i < sizes[0]; i++) {
        biganswer[i] = answers[0][i];
    }

    
    //debug
    // for (int i = 0; i < sizes[0]; i++) {
    //     cout<<biganswer[i]<<endl;
    // }
   //debug

    
    protocol.send(biganswer, sizes[0], reply_fd);

    for (int i = 0; i < numWorkers; i++) {
        close(s_to_w[i]);
    }
       for (int i = 0; i < numWorkers; i++) {
        delete [] answers[i];
    }


    delete[] question;
    delete[] biganswer;


}

///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

void Server::shutdown(int numWorkers) {
    cout << "Server::shutdown " << endl;
    for (int i = 0; i < numWorkers; i++) {
        Server::queue->place(NULL);
    }
}

void Server::serviceMaster(WorkerThread *workers, int numWorkers, int bufferSize) {

    cout << "Waiting for summaries from master ... \n";

    struct sockaddr_in client;
    socklen_t clientlen = sizeof (client);

    int x = 0;

    while (Server::shutdown_flag == false) {
        if (Server::shutdown_flag == true) {
            break;
        } else {
            int master_fd;

            if ((master_fd = accept(ServerConfiguration::statistics_fd, (struct sockaddr *) &client, &clientlen)) < 0) {
                perror("accept");
                break;
            } else {
                cout << "A worker has connected \n";

                x++;
                //////////-------------------------------

                //////////////////////////////////////////

                Protocol protocol;

                cout << "Buffer size: " << protocol.bufferSize << "\n";

                int size = 0;
                string * answer = protocol.receive(master_fd, &size);

                int numWorkers = atoi(answer[0].c_str());
                cout << "answer from worker to server , num workers = " << numWorkers << endl;


                delete [] answer;



                int size2 = 0;
                string * answer2 = protocol.receive(master_fd, &size2);

                // debug answers
                // for(int i=0; i<size2; i++){
                //     cout<<"answer from worker ( to server ) :"<<answer2[i]<<endl;
                // }

                // ~debug answers

                if (x == 1) {
                    Server::masterWorkers = new MasterWorker[numWorkers];
                }

                // setMasterWorkerValues( string ip, int port , string* countries , int how_many_countries){
                int how_many_countries = size2 - 2;

                Server::masterWorkers[x - 1].setMasterWorkerValues(answer2[0], atoi(answer2[1].c_str()), how_many_countries, numWorkers);
                //if((size2-2)>0)
                //to size-2 einai giati sth 8esh 0 kai 1 einai ip kai port antistoixa 
                for (int i = 0; i < (size2 - 2); i++) {
                    //    cout<<"mphke i fores"<<(i+1)<<endl;
                    Server::masterWorkers[x - 1].setMasterWorkerCountries(answer2[i + 2], i);
                    masterWorkers[x - 1].printAtIndexMasterWorkerCountries(i);
                }

                //inet_addr isws voleuei to apo8hkeuw ap eu8eias etsi sto masterworker object 8a dei3ei

                //                inet_addr(Server::masterWorkers[x - 1].getIp().c_str());


                if (x == numWorkers) {
                    for (int i = 0; i < numWorkers; i++) {
                        cout<<"index "<<i<<" "<<endl;
                        Server::masterWorkers[i].printAllValues();
                    }

                    break;
                }
            }
        }
    }
}

void Server::serviceClients(WorkerThread *workers, int numWorkers, int bufferSize) {

    cout << "*************************************\n";
    cout << "Waiting for queries from clients ... \n";
    cout << "*************************************\n";

    struct sockaddr_in client;
    socklen_t clientlen = sizeof (client);

    while (Server::shutdown_flag == false) {
        if (Server::shutdown_flag == true) {
            break;
        } else {
            int newsock;
            if ((newsock = accept(ServerConfiguration::queries_fd, (struct sockaddr *) &client, &clientlen)) < 0) {
                perror("accept");
                break;
            } else {
                cout << "A client has connected \n";
            }

            int * d = new int;
            *d = newsock;
            cout << "upoti8etai oti twra 8a kanei place to descriptor tou client \n\n";
            Server::queue->place((void*) d);
        }
    }
}

void *Server::child() {
    char line[2000];

    cout << "Child started " << pthread_self() << endl;

    while (true) {
        int * d = (int*) Server::queue->obtain();

        if (d == NULL) {
            return NULL;
        }

        cout << "Child (Client) connected: " << *d << endl;

        int size;

        Protocol protocol;
        string * request = protocol.receive(*d, &size);
        string command = *request;
        delete [] request;

        cout << "Command received: " << command << endl;

        strcpy(line, command.c_str());

        //response=spellchecker(command);
        //if answer!=null
        //protocol.send(response , x , *whatever);
        //else send unsupported opperation;

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

        bool supported = false;
        int numWorkers = Server::masterWorkers[0].how_many_workers;

        int c = sscanf(line, "%ms %ms %ms %ms %ms %ms %ms %ms", &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8);

        if (c1 == NULL) {
            printf("No input at all , please type a command OR type: exit.\n");            
            close(*d);
            continue;
        }

        if (strcmp(c1, "/diseaseFrequency") == 0) {
            // printf("num current Disease\n");

            supported = true;
            
            if (c == 4) { // without country
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                diseaseFrequency(*d, whatDisease, from, to, numWorkers);
            } else if (c == 5) { //WITH  COUNTRY
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                diseaseFrequency(*d, whatDisease, from, to, whatCountry);
            } else {
                printf("Error : diseaseFrequency needs Virus Date1 Date2 OR Virus Country Date1 Date2\n");
            }
        } else if (strcmp(c1, "/topk-AgeRanges") == 0) {
                        supported = true;
                       if (c == 6) {
                        //    printf("myshell : /topK-AgeRanges\n");
                           int k = atoi(c2);
                           string whatCountry = (c3);
                           string whatDisease = (c4);
                           Date from = Date(c5);
                           Date to = Date(c6);
                        //    printf("%d \n" , k);
                           cout << whatCountry << " " << whatDisease << endl;
                           topkAgeRanges(*d,k, whatDisease, from, to, whatCountry);
                       } else {
                           printf("myshell : error in the arguments for /topKAgeRanges needs to be 5 and it is %d\n", (int) c);
                       }
        } else if (strcmp(c1, "/searchPatientRecord") == 0) {
            //            if (c == 2) {
            //                printf("myshell : /searchPatientRecord\n");
            //            } else {
            //                printf("myshell : error in the arguments for/searchPatientRecord needs to be 2 and it is %d\n", (int) c);
            //            }
        } else if (strcmp(c1, "/numPatientDischarges") == 0) {
            supported = true;
            if (c == 4) { // without country
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                numPatientDischarges(*d, whatDisease, from, to, numWorkers);
            } else if (c == 5) { //WITH  COUNTRY
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientDischarges(*d, whatDisease, from, to, whatCountry);
            } else {
                printf("Error : numPatientDischarges needs Virus Date1 Date2 OR Virus Country Date1 Date2\n");
            }
        } else if (strcmp(c1, "/numPatientAdmissions") == 0) {
            supported = true;
            if (c == 4) { // without country
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                numPatientAdmissions(*d, whatDisease, from, to, numWorkers);
            } else if (c == 5) { //WITH  COUNTRY
                string whatDisease(c2);
                Date from = Date(c3);
                Date to = Date(c4);
                string whatCountry(c5);
                numPatientAdmissions(*d, whatDisease, from, to, whatCountry);
            } else {
                printf("Error : numPatientAdmissions needs Virus Date1 Date2 OR Virus Country Date1 Date2\n");
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



        if (!supported) {
            string response[1] = {"unsupported operation "};
            protocol.send(response, 1, *d);
        }

        close(*d);
    }
}




WorkerThread *Server::setupCommunication(int statisticsPortNum, int queryPortNum, int numWorkers, int poolSize) {

    struct sockaddr_in server_stats;
    struct sockaddr *server_stats_ptr = (struct sockaddr *) &server_stats;

    if ((ServerConfiguration::statistics_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
    }

    server_stats.sin_family = AF_INET; /* Internet domain */
    server_stats.sin_addr.s_addr = htonl(INADDR_ANY);
    server_stats.sin_port = htons(statisticsPortNum); /* The given port */

    if (bind(ServerConfiguration::statistics_fd, server_stats_ptr, sizeof (server_stats)) < 0) {
        perror("bind");
        exit(1);
    }


    // int x = 1;
    // if (setsockopt(ServerConfiguration::statistics_fd, SOL_SOCKET, SO_REUSEADDR, &x, sizeof (int)) < 0) {
    //     perror("setsockopt(SO_REUSEADDR) failed");
    //     exit(1);
    // }

    if (listen(ServerConfiguration::statistics_fd, 250) < 0) {
        perror("listen");
    }

    struct sockaddr_in server_query;
    struct sockaddr *server_query_ptr = (struct sockaddr *) &server_query;

    if ((ServerConfiguration::queries_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
    }

    server_query.sin_family = AF_INET; /* Internet domain */
    server_query.sin_addr.s_addr = htonl(INADDR_ANY);
    server_query.sin_port = htons(queryPortNum); /* The given port */

    if (bind(ServerConfiguration::queries_fd, server_query_ptr, sizeof (server_query)) < 0) {
        perror("bind");
        exit(1);
    }


    // int xx = 1;
    // if (setsockopt(ServerConfiguration::queries_fd, SOL_SOCKET, SO_REUSEADDR, &xx, sizeof (int)) < 0) {
    //     perror("setsockopt(SO_REUSEADDR) failed");
    //     exit(1);
    // }

    //to 5 einai to orio apo tcp sundeseis 
    if (listen(ServerConfiguration::queries_fd, 5) < 0) {
        perror("listen");
    }





    WorkerThread *workers = new WorkerThread[numWorkers];

    for (int i = 0; i < numWorkers; i++) {
        workers[i].id = i;
        workers[i].tid = 0;

    }

    return workers;
}

void Server::closeCommunication(int numWorkers, WorkerThread *workers) {
    close(ServerConfiguration::queries_fd);

    close(ServerConfiguration::statistics_fd);

    for (int i = 0; i < numWorkers; i++) {
        if (workers[i].tid != 0) {
            pthread_join(workers[i].tid, 0);
        }

    }

}
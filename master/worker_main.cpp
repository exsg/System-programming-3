#include <string>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>      /* sockets */
#include <netinet/in.h>      /* internet sockets */
#include <netdb.h>

#include <cstring>          /* gethostbyaddr */

#include <arpa/inet.h>

#include "WorkerShell.h"
#include "Country.h"
#include "Date.h"
#include "Protocol.h"

#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mvar;

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

int main_worker(string inputdir, int w_to_a, int a_to_w, int numWorkers) {
    int howmany_countries = 0;
    int howmany_connection_data = 0;

    cout << "Worker started with pid: " << getpid() << "\n";

    string * countries = Protocol::receive(a_to_w, &howmany_countries);

    for (int i = 0; i < howmany_countries; i++) {
        Country::add(new Country(countries[i]));
        cout << countries[i];
    }


    Country::print();

    string * connectiondata = Protocol::receive(a_to_w, &howmany_connection_data);

    for (int i = 0; i < howmany_countries; i++) {
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

    cout << "howmany countries for this worker: " << howmany_countries << endl;

    Date * lastDates = new Date[howmany_countries];


    for (int i = 0; i < howmany_countries; i++) {
        // string s = countries[i];
        lastDates[i] = Country::findLastDateForCountry(countries[i]);
        // printf("latest date for %s is ", countries[i].c_str());
        // printDate(lastDates[i]);
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

    // int port;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof (client);

    if ((WorkerShell::incoming_socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    server.sin_family = AF_INET; /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(0); /* The given port */

    //--------------------------------------
    client.sin_family = AF_INET; /* Internet domain */
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(0); /* The given port */
    //--------------------------------------
    if (bind(WorkerShell::incoming_socket_fd, (struct sockaddr *) &server, sizeof (server)) < 0) {
        return -1;
    }

    if (listen(WorkerShell::incoming_socket_fd, 500) < 0) {
        return -1;
    }

    cout << "Worker prepared !!!! Server Connection data: " << connectiondata[0] << ":" << connectiondata[1] << endl;


    ssize_t n = write(w_to_a, (char*) &reply, sizeof (unsigned char));


    if (n != sizeof (unsigned char)) {
        cout << "Worker: could not talk to aggregator, reply sent failed" << endl;
        exit(n);
    } else {
        close(w_to_a);
        close(a_to_w);
    }


    string server_ip = connectiondata[0];
    int server_port = atoi(connectiondata[1].c_str());


    int server_fd;
    struct sockaddr_in server_out;
    struct hostent *rem;

    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    if ((rem = gethostbyname(server_ip.c_str())) == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    server_out.sin_family = AF_INET; /* Internet domain */
    memcpy(&server_out.sin_addr, rem->h_addr, rem->h_length);


    server_out.sin_port = htons(server_port); /* Server port */

    if (connect(server_fd, (struct sockaddr*) &server_out, sizeof (server_out)) < 0) {
        perror("connect");
        exit(1);
    }


    string * answer = new string[1];
    answer[0] = Protocol::to_string(numWorkers);

    Protocol::send(answer, 1, server_fd);

    delete [] answer;

    // ##### send ip
    //na dw an xreiazetai kati apo auta memcpy
    char myIP[16];

    struct sockaddr_in my_addr;
    pthread_mutex_lock(&mvar);

    bzero(&my_addr, sizeof (my_addr));
    int len = sizeof (my_addr);
    getsockname(server_fd, (struct sockaddr *) &my_addr, (socklen_t*) & len); //me to getsockname pairnw thn ip apo thn pleura mou
    //enw me thn getpeername pairnw thn ip apo thn allh pleura ( tou kanonikoy server )
    inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof (myIP)); //metafrash kai apo8hkeush sto my_addr

    printf("Local ip address: %s\n", myIP);

    //////////////////////////23-6-2020
    //na dw an pairnei  to port tou client h to port tou server
    //auto den leitourgei
    // int x=ntohs(server.sin_port);
    // printf("int x = ntohs(server.sin_port) =%d \n" , x);
    // cout<<"\n\n\n-------------------------------\n";






    int Serverlen = sizeof (server);
    if (getsockname(WorkerShell::incoming_socket_fd, (struct sockaddr *) &server, (socklen_t*) & Serverlen) < 0) { /* Selected port */
        perror("getsockname");
        exit(1);
    }

    //peirama me ip apo to WorkerShell::incoming_socket_fd
    // char myIP2[16];
    // inet_ntop(AF_INET, &server.sin_addr, myIP2, sizeof(myIP2));
    // printf("-------------IP apo WorkerShell::incoming_socket_fd\n");
    // printf("IP apo thn pleura tou worker= %s\n" , myIP2);
    // printf("-------------IP apo WorkerShell::incoming_socket_fd\n\n\n");
    //telos peiramatos , vgazei 0.0.0.0 , den moy kanei alla to krataw gia reference

    printf("Socket port: %d\n", ntohs(server.sin_port));
    int x = ntohs(server.sin_port);
    printf("int x = ntohs(server.sin_port) =%d \n", x);
    cout << "\n\n\n-------------------------------\n";





    ///////////////////////////23-6-2020

    //////////////////////////////

    string * answer2 = new string[2 + howmany_countries];
    answer2[0] = string(myIP); //apo getsockname me server_fd 
    answer2[1] = to_string(x);
    //exw string countries p exw ftia3ei apo thn prohgoumenh askhsh gia na ftia3w th lista countries.
    for (int i = 0; i < howmany_countries; i++) {
        printf("for worker with ip %s port %d , country %d ", myIP, x, i);
        cout << countries[i] << endl;
        answer2[i + 2] = countries[i]; // to +2 profanws to exw giati sto 0 kai 1 exw ip kai port
    }

    Protocol::send(answer2, (2 + howmany_countries), server_fd);

    delete [] answer2;
    ///////////////////////////////

    pthread_mutex_unlock(&mvar);

    char hisIP[16];

    struct sockaddr_in his_addr;
    pthread_mutex_lock(&mvar);
    bzero(&his_addr, sizeof (his_addr));
    int len2 = sizeof (his_addr);
    //etsi pairnw tou server thn ip. mporw na to kanw apo thn pleura tou server gia na parw thn ip tou client.
    getpeername(server_fd, (struct sockaddr *) &his_addr, (socklen_t*) & len2);
    inet_ntop(AF_INET, &his_addr.sin_addr, hisIP, sizeof (hisIP));

    printf("Servers ip address: %s\n", hisIP);

    pthread_mutex_unlock(&mvar);
    // myPort = ntohs(my_addr.sin_port);

    // struct hostent *rem2;

    // if ((rem2 = gethostbyname("255.255.255.255")) == NULL) {
    //     herror("gethostbyname");
    //     exit(1);
    // }
    // pthread_mutex_lock(&mvar);
    //auto vgazei to idio akrivws me auto p exw valei sto gethostbyname
    // printf("*********************************\n %s\n************************" , rem2->h_name);
    // struct in_addr **addr_list; 
    // addr_list= (struct in_addr **)rem2->h_addr_list;
    // for (int i =0; addr_list[i]!=NULL; i++){
    //     printf(" IP addresses : ");
    //     cout<<inet_ntoa(*addr_list[i])<<endl;
    // }
    //auto vgazei thn ip an valw sto gethostbyname px google.

    // pthread_mutex_unlock(&mvar);



    ///////peirama , anoigw neo skocket apo katw
    // int Client_socket_fd;


    // if ((Client_socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    //     perror("socket");
    // }
    // client.sin_family = AF_INET;                /* Internet domain */
    // client.sin_addr.s_addr = htonl(INADDR_ANY); /* My Internet address */
    // client.sin_port = htons(0);                /* Select any port */

    //  if (bind(Client_socket_fd, (struct sockaddr *) &client,sizeof(client) ) < 0) {
    //     return -1;
    // }

    //  if (listen(Client_socket_fd,500 ) < 0) {
    //     return -1;
    // }

    // pthread_mutex_lock(&mvar);


    //   if (getsockname(Client_socket_fd,(struct sockaddr * ) &client, ( socklen_t*)&clientlen) < 0)
    // { /* Selected port */
    //     perror("getsockname");
    //     exit(1);
    // }

    // printf("Socket port: %d\n", ntohs(client.sin_port));
    // int x=ntohs(client.sin_port);
    // printf("int x = ntohs(client.sin_port) =%d \n" , x);
    // cout<<"\n\n\n-------------------------------\n";



    // pthread_mutex_unlock(&mvar);

    ///////peirama , anoigw neo skocket apo panw

    //  if (bind(Client_socket_fd, (struct sockaddr *) &client, clientlen) < 0) {
    //     return -1;
    // }

    // if (listen(Client_socket_fd, 500) < 0) {
    //     return -1;
    // }


    // ##### send port

    // int sock;
    // cout<<"\n\n\n-------------------------------\n";
    // cout<<"(int)server_out.sin_port : \n"<<(int)client.sin_port<<endl;
    // cout<<"ntohs(server_out.sin_port) : "<<(int)ntohs(client.sin_port)<<" "<<endl;
    //   if (getsockname(server_fd,(struct sockaddr * ) &client, ( socklen_t*)&clientlen) < 0)
    // { /* Selected port */
    //     perror("getsockname");
    //     exit(1);
    // }
    // printf("Socket port: %d\n", ntohs(client.sin_port));
    // int x=ntohs(client.sin_port);
    // printf("int x = ntohs(client.sin_port) =%d \n" , x);
    // cout<<"\n\n\n-------------------------------\n";


    // 


    // ##### send countries









    close(server_fd);

    while (true) {
        int newsock;
        if ((newsock = accept(WorkerShell::incoming_socket_fd, (struct sockaddr *) &client, &clientlen)) < 0) {
            perror("accept");
            break;
        }

        WorkerShell::shell(newsock, newsock);
        
        close(newsock);
    }

    //    WorkerShell::shell(w_to_a, a_to_w);

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

#include "MasterWorker.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
// isws volepsei na exw to howmany countries ws static ths klasshs
//MasterWorker::how_many_countries=0
//kai na ana8etw timh otan lavei o server to posoi einai apo ton worker tou master
// etsi wste na mhn xrhsimpoiw gia ka8e MasterWorker object xwro gia thn idia metavlhth 

MasterWorker::MasterWorker() {
    countries = NULL;
    id = 0;
    ip = "";
    port = 0;
    how_many_countries = 0;
}

//na ftia3w destructor!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// void MasterWorker::setMasterWorkerValues(int id1  , string ip1, int port1 , string* countries1 , int how_many_countries1){
//     id=id1;
//     ip=ip1;
//     port=port1;
//     how_many_countries=how_many_countries1;
//     for(int i=0; i<how_many_countries;i++){
//         countries[i]=countries[i];
//     }
// }

// void MasterWorker::setMasterWorkerValues( string ip1, int port1 , string* countries1 , int how_many_countries1){
//     // id=id;
//     ip=ip1;
//     port=port1;
//     how_many_countries=how_many_countries1;
//     for(int i=0; i<how_many_countries1;i++){
//         countries[i]=countries1[i];
//     }
// }

void MasterWorker::setMasterWorkerValues(string ip1, int port1, int how_many_countries, int how_many_workers) {
    ip = ip1;
    port = port1;
    this->how_many_countries = how_many_countries;
    this->how_many_workers = how_many_workers;

    countries = new string[this->how_many_countries];
}

void MasterWorker::setMasterWorkerCountries(string countries1, int index) {
    countries[index] = countries1;
}

void MasterWorker::setMasterWorkerCountries(string * countries1) {
    for (int i = 0; i < how_many_countries; i++) {
        countries[i] = countries1[i];
    }
}

void MasterWorker::printAtIndexMasterWorkerCountries(int index) {
    cout << countries[index] << endl;
}

void MasterWorker::printCountries() {
    for (int i = 0; i < how_many_countries; i++) {
        cout << countries[i] << endl;
    }
}

void MasterWorker::printAllValues() {
    cout << "MasterWorker::printAllValues : ";
    cout << "ip " << ip;
    printf(", port %d, #of countries %d, total workers: %d \n", port, how_many_countries, how_many_workers);
    cout << "this worker has the following countries : \n";
    printCountries();
}

string MasterWorker::getIp() {
    return ip;
}

int MasterWorker::getHowManyCountries() {
    return how_many_countries;
}

string* MasterWorker::getCountries() {
    return countries;
}

int MasterWorker::getPort() {
    return port;
}



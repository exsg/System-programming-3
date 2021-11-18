#ifndef MASTERWORKERTHREAD_H
#define MASTERWORKERTHREAD_H

#include <string>

using namespace std;

class MasterWorker{
public:
    MasterWorker();
    int how_many_workers;
    int how_many_countries;
    int id;
    // pthread_t tid;
    string * countries;
    string ip;
    int port;
    //setters
    // void setMasterWorkerValues(int id1  , string ip1, int port1 , string* countries1 , int how_many_countries1);
    // void setMasterWorkerValues( string ip1, int port1 , string* countries1 , int how_many_countries1);
    void setMasterWorkerValues( string ip1, int port1  , int how_many_countries1, int how_many_workers);
    void setMasterWorkerCountries(string countries1 , int index);
    void setMasterWorkerCountries(string * countries1);
    //getters
    string getIp();
    string* getCountries();
    int getPort();
    int getHowManyCountries();
    //debug prints
    void printAtIndexMasterWorkerCountries(int index);
    void printCountries();
    void printAllValues();
};

#endif /* MASTERWORKERTHREAD_H*/


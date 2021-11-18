

#ifndef AGGREGATOR_SHELL_H
#define AGGREGATOR_SHELL_H

#include <string>

#include "Date.h"
#include "Patient.h"
#include "Worker.h"

using namespace std;

class AggregatorShell {
public:
    
    static Worker * workers;

    static Patient* searchPatientRecord(string id); 
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    
    static void diseaseFrequency(string whatDisease, Date from, Date to, string country);

    static void diseaseFrequency(string whatDisease, Date from, Date to ,int numWorkers);
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    
    static void numPatientAdmissions(string whatDisease, Date from, Date to, string country);
    
    static void numPatientAdmissions(string whatDisease, Date from, Date to ,int numWorkers);
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    
    static void numPatientDischarges(string whatDisease, Date from, Date to, string country);
    
    static void numPatientDischarges(string whatDisease, Date from, Date to , int numWorkers);
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    
    static void topkAgeRanges(int k, string whatDisease, Date from, Date to, string country);
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```
    
    static Worker * setupCommunication(string whichDirectory, int numWorkers, int b, string ip , int port);
    
      static void shutdown();
      
    static void shell(Worker * workers , int numWorkers, int b);
    
    static void closeCommunication(int numWorkers, Worker * workers);

    static bool shutdown_flag;
};
#endif


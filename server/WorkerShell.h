

#ifndef SHELL_H
#define SHELL_H

#include <string>

#include "Date.h"
#include "Patient.h"

using namespace std;

class WorkerShell {
public:

    static void insertPatientRecord(Patient * pn);
    
    static Patient* searchPatientRecord(string id); 
    static void diseaseFrequency(string whatDisease, Date from, Date to, string country);

    static void diseaseFrequency(string whatDisease, Date from, Date to);
    
    static void numPatientAdmissions(string whatDisease, Date from, Date to, string country);
    
    static void numPatientAdmissions(string whatDisease, Date from, Date to);
    
    static void numPatientDischarges(string whatDisease, Date from, Date to, string country);
    
    static void numPatientDischarges(string whatDisease, Date from, Date to);
    
    static void topkAgeRanges(int k, string whatDisease, Date from, Date to, string country);
    
    static void reload(string inputdir);
    
    static void shutdown();
    
    static void shell(int w_to_a, int a_to_w );

    static int w_to_a;
    static int a_to_w ;
};

int main_worker(string inputdir, int w_to_a, int a_to_w );
#endif


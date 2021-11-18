#ifndef PATIENT_H
#define PATIENT_H

#include <string>

#include "Date.h"

using namespace std;

class Patient {
public:
    string id;
    
    string firstName;
    string lastName;
    string disease;    
    string country;
    Date entryDate;
    Date exitDate;
    int age;
    
    struct Patient* nextPatient;
    
    // ----------------------------------------
    
    static Patient * patientsHead;
    static Patient * completePatientList;
    static Patient* returnPatientId(string id);
    static void findPatientTwice();
    static Patient* returnCompletePatient();
    static void checkIfTwoTimes(string id);
    static bool checkIfInList(string id);
    static bool add(Patient * patient , char * status);
    static void add2(Patient *patient);
    static void printPatient(Patient* patient);
    static Patient* itteratePatientList(Patient *patient);
    
    static void destroy();
    
    static void readFile(string country, string date, string whichFile);
    
};
  
    

#endif /* PATIENT_H */


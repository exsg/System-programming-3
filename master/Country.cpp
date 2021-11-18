
#include "Country.h"
#include "Date.h"
#include "Patient.h"

#include <stdio.h>
#include <stdlib.h>

Country * Country::countriesHead = NULL;

void Country::add(Country * country) {
    if (countriesHead == NULL) {
        countriesHead = country;
    } else {
        bool duplicate = false;

        Country *temp = countriesHead;

        while (temp != NULL) {
            if (temp->name == country->name) {
                duplicate = true;
                break;
            }
            temp = temp->nextCountry;
        }

        if (!duplicate) {
            country->nextCountry = countriesHead;
            countriesHead = country;

        }
    }
}

Date Country::findLastDateForCountry(string country){
    Patient * thisP;
    thisP=Patient::completePatientList;
    Date dummyDate;
    dummyDate.iDD=-1;
    dummyDate.iMM=-1;
    dummyDate.iYYYY=-1;
    if(thisP!=NULL){
        Date mostRecentDate;
        if(compareDates(thisP->entryDate  ,thisP->exitDate )){
            mostRecentDate=thisP->entryDate;
        }else{
            mostRecentDate=thisP->exitDate;
        }

        while(thisP!=NULL){
            if(thisP->country==country){
                if(compareDates(thisP->entryDate  ,thisP->exitDate)){
                    if(compareDates(thisP->entryDate , mostRecentDate)){
                        mostRecentDate=thisP->entryDate;
                    }
                }else{
                    if(compareDates(thisP->exitDate , mostRecentDate)){
                        mostRecentDate=thisP->exitDate;
                    }
                }
            }

            thisP=thisP->nextPatient;
        }
        return mostRecentDate;
    }else{
        
        return dummyDate;
    }

}

void Country::destroy() {
    cout << "Destroying countries \n";

    Country *temp = countriesHead;

    while (temp != NULL) {
        Country *victim = temp;
        temp = temp->nextCountry;
        delete victim;
    }
}

void Country::print() {
    Country *temp = countriesHead;

    while (temp != NULL) {
        cout << " -" << temp->name << endl;
        temp = temp->nextCountry;
    }
}

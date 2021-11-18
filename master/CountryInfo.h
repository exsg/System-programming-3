#ifndef COUNTRY_INFO_H
#define COUNTRY_INFO_H

#include <iostream>
#include <string>

#include "Worker.h"

using namespace std;

class CountryInfo {
public:
    string name;
    int worker_number;

    CountryInfo * nextCountryInfo;

    CountryInfo(string name, int worker_number) : name(name), worker_number(worker_number), nextCountryInfo(NULL) {

    }

    // ----------------------------------------

    static CountryInfo * countriesHead;

    static void add(CountryInfo * country);

    static void destroy();
    
    static void print();
    //poses xwres exei autos o worker
    static int  countCountriesForWorker(int i);
    //poses einai sunolika gia olous tous workers
    static int  countAllCountries();
    
    //gurizei pinaka me countries ( string )
    static string * getCountries(int i);
    //gurizei nea metavlhth worker pou isoutai me ton worker poy tou antistoixei ayth h xwra; 
    static Worker * getWorker(Worker * workers, string country);
};

#endif 


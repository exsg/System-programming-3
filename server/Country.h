#ifndef COUNTRY_H
#define COUNTRY_H

#include <iostream>
#include <string>
#include "Date.h"

using namespace std;

class Country {
public:
    string name;

    Country * nextCountry;

    Date lastDate;

    Country(string name) : name(name), nextCountry(NULL) {

    }

    // ----------------------------------------

    static Country * countriesHead;

    static void add(Country * country);

    static void destroy();
    
    static void print();
    static Date findLastDateForCountry(string country);
};

#endif /* COUNTRY_H */


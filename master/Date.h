#ifndef DATE_H
#define DATE_H

#include <string>

using namespace std;

class Date {
public:
    int iMM;
    int iDD;
    int iYYYY;
    string date;

    Date() : date("") {
        iMM = 0;
        iDD = 0;
        iYYYY = 0;
    }

    Date(string date) : date(date) {
        char DD[3] = {' ', ' ', '\0'};
        char MM[3] = {' ', ' ', '\0'};
        char YYYY[5] = {' ', ' ', ' ', ' ', '\0'};

        if (date.length() > 3) {
            DD[0] = date[0];
            DD[1] = date[1];
            MM[0] = date[3];
            MM[1] = date[4];
            YYYY[0] = date[6];
            YYYY[1] = date[7];
            YYYY[2] = date[8];
            YYYY[3] = date[9];

            iDD = atoi(DD);
            iMM = atoi(MM);
            iYYYY = atoi(YYYY);
        } else {
            iDD = -1;
            iMM = -1;
            iYYYY = -1;
        }
    }
};
void printDate(Date date);
bool compareDates(Date date1, Date date2);

#endif 


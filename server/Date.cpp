#include "Date.h"
#include <iostream>

//gurizei true an Date1>=date2 
bool compareDates(Date date1 , Date date2){
    return ((date2.iYYYY < date1.iYYYY) || 
            ((date2.iYYYY == date1.iYYYY)&&(date2.iMM < date1.iMM)) || 
            ((date2.iYYYY == date1.iYYYY)&&(date2.iMM == date1.iMM)&&(date2.iDD <= date1.iDD)));
}

void printDate(Date date){
    std::cout<<date.iDD<<"-"<<date.iMM<<"-"<<date.iYYYY<<endl;
}

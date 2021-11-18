#ifndef WHOCLIENTREAD_H
#define WHOCLIENTREAD_H

#include <string>
#include <string.h>
using namespace std;

 typedef struct  StringArray{
    string* sArray;
    int size;
} StringArray;

int* createInt(int num);

string* clientReadFile(string whichFile , int * index);

#endif
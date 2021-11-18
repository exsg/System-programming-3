#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "whoClientRead.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

int* createInt(int num) {
    int * numpointer = (int*) malloc(sizeof (int));
    *numpointer = num;
    cout << "to num poy phre =" << num << endl;
    cout << "8esh sth mnhmh " << numpointer << endl;
    return numpointer;
}

string*  clientReadFile(string whichFile , int* index) {

    FILE *fp = fopen(whichFile.c_str(), "r");

    if (fp == NULL) {
        // perror("Error ");
        return NULL;
    } else {

        string wholeQuery = "";

        char buffer[100];

        int i = 0;

        string * stringArray;
        stringArray = NULL;
        // stringArray = (string*) malloc(sizeof (string));
       stringArray= new string;
       
        
        while (fgets(buffer, 100, fp)) {
            i++;
            wholeQuery = string(buffer);
            if (i == 1) {
                
                stringArray[i - 1] = wholeQuery;
            } else {
                stringArray = (string*) realloc(stringArray, sizeof (string) * (i+1));
                stringArray[i - 1] = wholeQuery;
            }
            //debug
            // cout<<i;
            // cout<<wholeQuery<<endl;

        }
        cout << "from who client i = " << i << endl;
        // struct StringArray *thisArray;
        // thisArray = (StringArray*) malloc(sizeof (StringArray));
        // thisArray->sArray = (string*) malloc(sizeof (string));
        // thisArray->sArray = stringArray;
        // thisArray->size = i;
        *index=i;



        //debug
        // for(int j=0; j<thisArray->size;j++){
        //     cout<<thisArray->sArray[j];
        // }

        

        fclose(fp);
        return stringArray;
    }
    
    // return StringArray();
}
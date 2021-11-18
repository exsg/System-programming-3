#include <iostream>
#include <string>

#include "Patient.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

Patient *Patient::patientsHead = NULL;
Patient *Patient::completePatientList = NULL;

Patient *Patient::returnPatientId(string id)
{

    Patient *temp = new Patient();

    temp = Patient::patientsHead;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }
        temp = temp->nextPatient;
    }

    return NULL;
}

bool Patient::add(Patient *patient, char *status)
{
    if (patientsHead == NULL)
    {
        patientsHead = patient;
        return true;
    }
    else
    {
        bool duplicate = false;

        Patient *temp = patientsHead;

        while (temp != NULL)
        {
            if (temp->id == patient->id)
            {
                duplicate = true;
                break;
            }
            temp = temp->nextPatient;
        }

        if (!duplicate)
        {
            if (string(status) == "ENTER")
            {
                patient->nextPatient = patientsHead;
                patientsHead = patient;
                return true;
            }
            else
            {
                cout << "Patient::add error, Patient found first time with only exit date \n";
                return false;
            }
        }
        else
        { // twra o patient einai hdh sth lista kai prepei na dw an exei entry date.
            // TODO  if patient has not exited, set exit date else ignore
            if (string(status) == "ENTER")
            {
                printf("Patient::add error , patient found in list with entry date \n");
                return false;
            }
            else
            {
                if (temp->entryDate.iDD != 0)
                {
                    temp->exitDate = patient->exitDate;
                    return true;
                }
                else
                {
                    cout << "Patient::add error , patient tries to exit without entry date\n";
                    return false;
                }
            }
        }
    }
}
void Patient::add2(Patient *patient)
{
    int i = 0;
    if (patientsHead == NULL)
    {
        patientsHead = patient;
        // return true;
        return;
    }
    else
    {
        // bool duplicate = false;

        Patient *curr = patientsHead;
        // Patient *previous=NULL;
        

        while (curr != NULL)
        {
            curr = curr->nextPatient;
            i++;
        }

        // 
        patient->nextPatient = patientsHead;
        patientsHead = patient;
    }
    // cout << "list with no complete patient #of nodes=" << i << endl;
}
void Patient::checkIfTwoTimes(string id)
{
    Patient *p;
    p = completePatientList;
    int flag = 0;
    while (p != NULL)
    {
        if (p->id == id)
        {
            flag++;
        }
        p = p->nextPatient;
    }
    if (flag == 2)
    {
        printf("double entry\n");
    }
}

bool Patient::checkIfInList(string id)
{
    Patient *p;
    p = completePatientList;

    while (p != NULL)
    {
        if (id == p->id)
        {
            return true;
        }
        p = p->nextPatient;
    }
    return false;
}



// Patient *Patient::returnCompletePatient(Patient *thisP)
// {

//     if (thisP != NULL)
//     {
//         Patient *temp = new Patient();
//         Patient *completePatient = new Patient();

//         temp = patientsHead;
//         int flag = 0;
//         while (temp != NULL)
//         {
//             if ((temp->id == thisP->id))
//             {
//                 if (flag == 0)
//                 {
//                     flag++;
//                     completePatient = temp;
//                 }
//                 else
//                 {
//                     if (temp->entryDate.iDD > 0)
//                     {
//                         completePatient->entryDate = temp->entryDate;
//                         if ((completePatient->entryDate.iDD > 0) && (completePatient->exitDate.iDD > 0))
//                         {
//                             if (compareDates(completePatient->exitDate, completePatient->entryDate))
//                             {
//                                 //success++?
//                                 return completePatient;
//                             }
//                             else
//                             {
//                                 cout << "Patient::returnCompletePatient error ,Patient could not be inserted because exit date was before entry date";
//                                 //error++?
//                             }
//                         }
//                         else
//                         {
//                             cout << "Patient::returnCompletePatient error ,Patient had two entry or exit dates\n";
//                         }
//                     }
//                     else
//                     {
//                         completePatient->exitDate = temp->exitDate;
//                         if (temp->entryDate.iDD > 0)
//                         {
//                             completePatient->entryDate = temp->entryDate;
//                             if ((completePatient->entryDate.iDD > 0) && (completePatient->exitDate.iDD > 0))
//                             {
//                                 if (compareDates(completePatient->exitDate, completePatient->entryDate))
//                                 {
//                                     //success++?
//                                     return completePatient;
//                                 }
//                                 else
//                                 {
//                                     cout << "Patient::returnCompletePatient error ,Patient could not be inserted because exit date was before entry date";
//                                     //error++?
//                                 }
//                             }
//                             else
//                             {
//                                 cout << "Patient::returnCompletePatient error ,Patient had two entry or exit dates\n";
//                             }
//                         }
//                     }
//                 }
//                 temp = temp->nextPatient;
//             }
//             if(completePatient->entryDate.iDD>0){
//                 //success++
//                 return completePatient;
//             }else{
//                 cout<<"error Patient had exit Date without entry ";
//                 //error++
//             }
//         }

//     }else
//     {
//         cout << "Patient::returnCompletePatient error , null ptr given\n";
//     }
// }

Patient *Patient::returnCompletePatient()
{
    // Patient * thisP = new Patient();
    Patient *temp1;
    Patient *temp2;
    int success=0;
    int fail=0;

    // Patient *temp4 = new Patient();
    // Patient *completePatient = new Patient();
    // completePatient->nextPatient = NULL;

    temp1 = patientsHead;
    temp2 = patientsHead;

    // int i = 0;
    int flag = 0;
    while (temp1 != NULL)
    {
        Patient *completePatient = new Patient();
        while (temp2 != NULL)
        {

            if (temp1->id == temp2->id)
            {
                flag++;
                if (flag == 1)
                {
                    // completePatient=temp2;

                    completePatient->id = temp2->id;
                    completePatient->firstName = temp2->firstName;
                    completePatient->lastName = temp2->lastName;
                    completePatient->age = temp2->age;
                    completePatient->country = temp2->country;
                    completePatient->disease = temp2->disease;
                    // completePatient->nextPatient=new Patient();
                    completePatient->nextPatient = NULL;
                    if (temp2->entryDate.iDD > 0)
                    {
                        completePatient->entryDate = temp2->entryDate;
                    }
                    else
                    {
                        completePatient->exitDate = temp2->exitDate;
                    }
                }
                else if (flag == 2)
                {
                    if (temp2->entryDate.iDD > 0)
                    {
                        completePatient->entryDate = temp2->entryDate;
                        
                    }
                    else
                    {
                        completePatient->exitDate = temp2->exitDate;
                    }
                    if (compareDates(completePatient->exitDate, completePatient->entryDate))
                    {
                        if (!checkIfInList(completePatient->id))
                        {

                            if (completePatientList != NULL)
                            {
                                //8ewrhtika periexei thn prwth fora ton complete patient apo to prohgoumeno loop
                                completePatient->nextPatient = completePatientList;
                                completePatientList = completePatient;
                                success++;
                                // printPatient(completePatient);
                                break;
                            }
                            else
                            {
                                // completePatient->nextPatient=NULL;
                                // h lista einai adeia
                                completePatientList = completePatient;
                                success++;
                                // printPatient(completePatient);
                                break;
                            }
                        }else{
                            //den 8a to metraw gia fail , etsi kai alliws tous patients pou ftiaxnei 
                            //8a tous vrei 2 fores me auton ton algori8mo kai 8a tsekarei na dei an 
                            //hdh einai sth lista.Auto to fail vgazei sxedon ta idia fail me success 
                            // fail++;
                        }
                    }
                    else
                    {
                    // cout << "patient found with bad entry date\n";
                        fail++;
                    }
                }
            }
            temp2 = temp2->nextPatient;
        }

        if (flag == 1)
        {
            if (completePatient->entryDate.iDD > 0)
            {
                if (!checkIfInList(completePatient->id))
                {

                    if (completePatientList == NULL)
                    {
                        completePatientList = completePatient;
                        success++;
                    }
                    else
                    {
                        completePatient->nextPatient = completePatientList;
                        completePatientList = completePatient;
                        success++;
                    }
                }
            }
            else
            {
                cout << "patient found only with exit date\n";
                fail++;
            }
        }
        flag = 0;
        temp2 = patientsHead;
        temp1 = temp1->nextPatient;
    }
    // itteratePatientList(completePatientList);

    // printf("apo return complete list i=%d\n", i);
    printf("Success=%d\nFails=%d\n", success , fail);
    
    return NULL;
}
void Patient::printPatient(Patient *patient)
{
    if (patient != NULL)
    {
        cout << patient->id + " " + patient->firstName + " " + patient->lastName + " " + patient->disease + " " + patient->country;
        printf(" %d ", patient->age);
        printf(" entry :");
        printf(" %d-%d-%d ", patient->entryDate.iDD, patient->entryDate.iMM, patient->entryDate.iYYYY);
        if (patient->exitDate.iDD > 0)
        {
            printf(" %d-%d-%d ", patient->exitDate.iDD, patient->exitDate.iMM, patient->exitDate.iYYYY);
        }
        else
        {
            printf(" NO EXIT DATE");
        }
        printf("\n");
    }
    else
    {
        printf("Patient:: printPatient error , NULL given\n");
    }
}

Patient *Patient::itteratePatientList(Patient *patient)
{

    // Patient *p = new Patient();
    Patient *p;
    p = patient;
    int i = 0;
    while (p->nextPatient != NULL)
    {
        i++;
        printPatient(p);
        // cout << "i=" << i << endl;
        p = p->nextPatient;
    }
    if (p != NULL)
    {
        i++;
        printPatient(p);
        // cout << "i=" << i << endl;
    }
    return NULL;
}

void Patient::destroy()
{
}

void Patient::readFile(string country, string date, string whichFile)
{
    int c;

    // cout << "trying to read from " << whichFile << ", country is " << country << "  and date is " << date << " \n";

    FILE *fp = fopen(whichFile.c_str(), "r");

    if (fp == NULL)
    {
        perror("Error ");
        exit(-1);
    }

    char *id = NULL;
    char *firstName = NULL;
    char *lastName = NULL;
    char *whatDisease = NULL;
    char *status = NULL;

    int age;
    // int success = 0;
    // int error = 0;

    while ((c = fscanf(fp, "%ms %ms %ms %ms %ms %d", &id, &status, &firstName, &lastName, &whatDisease, &age)) == 6)
    {
        Patient *p = new Patient();
        p->id = id;
        p->firstName = firstName;
        p->lastName = lastName;
        p->disease = whatDisease;
        p->country = country;
        p->age = age;

        if (string(status) == "ENTER")
        {
            p->entryDate = Date(date);
            p->exitDate = Date();
        }
        else if (string(status) == "EXIT")
        {
            p->entryDate = Date();
            p->exitDate = Date(date);
        }
        else
        {
            cout << "status was not entry nor exit , exiting\n";
            exit(1);
        }

        // if (Patient::add(p ,  status)) {
        //     success++;
        // } else {
        //     error++;
        //     delete p;
        // }
        Patient::add2(p);
    }

    if (id != NULL)
    {
        free(id);
    }
    if (firstName != NULL)
    {
        free(firstName);
    }
    if (lastName != NULL)
    {
        free(lastName);
    }
    if (whatDisease != NULL)
    {
        free(whatDisease);
    }
    if (status != NULL)
    {
        free(status);
    }

    //ta errors kai ta success mporw na ta kanw eite ena class/struct ( me 2 integers ) kai na to pernaw me
    //pointer measa sto returnCompleteList eite na ta dinw san int* orismata gia na mporw na ta diaxeiristw kai ektos
    //tou function 

    // printf("Patients successfully inserted in list: %d \n", success);
    // printf("Patients failed to be inserted in list: %d \n", error);

    fclose(fp);
}
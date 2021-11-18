
#include <cstdio>


void printArgumentError() {
    printf("Error : Not enough Arguments\n");
    printf("Please insert -i [file name] -h1 [disease hash table number of entries(integer)] ");
    printf("\n-h2 [country hash table number of entries (int)] ");
    printf("-b [pipe buffer size Size (int)]\n");
};

void whichArgumentError(char* whichArgument) {
    printf("Error : inputs are wrong , try./[programName] -p [file].txt -h1 [integer] -h2 [integer] -b [integer]\n");
    printf("more specificly %s should have an integer value\n", whichArgument);
}
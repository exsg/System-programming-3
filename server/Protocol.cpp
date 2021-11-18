#include "Protocol.h"

#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

const unsigned Protocol::bufferSize = TCP_BUFFER_SIZE;

Protocol::Protocol() {
    buffer = new char[bufferSize];
}

Protocol::~Protocol() {
    delete [] (char*) buffer;
}

void Protocol::send(string * array, int size, int descriptor) {
    char header[1000];
    sprintf(header, "%d", size);
    string s = header;

    memset(buffer, 0, bufferSize);
    memcpy(buffer, (void*) s.c_str(), s.length());

    int n;

    do {
        n = write(descriptor, buffer, bufferSize);
    } while (n == -1 && errno == EINTR);


    if (n == -1) {
        perror("write failed in Protocol::send");
        exit(1);
    }
    if (n == 0) {
        perror("communication interrupted (EOF) in Protocol::send");
        exit(1);
    }

    if (n < (int) bufferSize) {
        perror("partial write in Protocol::send");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        string & s = array[i];

        if (s.length() < bufferSize) {
            memset(buffer, 0, bufferSize);
            memcpy(buffer, (void*) s.c_str(), s.length());

            int n;

            do {
                n = write(descriptor, buffer, bufferSize);
            } while (n == -1 && errno == EINTR);

            if (n == -1) {
                perror("write failed in Protocol::send");
                exit(1);
            }
            if (n == 0) {
                perror("communication interrupted (EOF) in Protocol::send");
                exit(1);
            }

            if (n < (int) bufferSize) {
                perror("partial write in Protocol::send");
                exit(1);
            }
        } else {
            cerr << "message too big " << endl;
            exit(1);
        }
    }
}

string Protocol::to_string(int v) {
    char buffer[1000];
    sprintf(buffer, "%d", v);

    return string(buffer);
}

string * Protocol::receive(int descriptor, int * size) {
    memset(buffer, 0, bufferSize);

    int n;

    do {
        n = read(descriptor, buffer, bufferSize);
    } while (n == -1 && errno == EINTR);

    if (n == -1) {
        perror("read failed in Protocol::receive");
        exit(1);
    }
    if (n == 0) {
        perror("communication interrupted (EOF) in Protocol::receive");
        exit(1);
    }

    if (n < (int) bufferSize) {
        perror("partial read in Protocol::receive #1");
        printf("n = %d \n", n);
        printf("bufferSize = %d \n", bufferSize);
        exit(1);
    }

    *size = atoi((const char *) buffer);

    string * array = new string[*size];


    for (int i = 0; i < *size; i++) {
        memset(buffer, 0, bufferSize);

        int n;

        do {
            n = read(descriptor, buffer, bufferSize);
        } while (n == -1 && errno == EINTR);

        if (n == -1) {
            perror("read failed in Protocol::receive");
            exit(1);
        }
        if (n == 0) {
            perror("communication interrupted (EOF) in Protocol::receive");
            exit(1);
        }

        if (n < (int) bufferSize) {
            perror("partial read in Protocol::receive #2");
            printf("n = %d \n", n);
            printf("bufferSize = %d \n", bufferSize);
            exit(1);
        }

        array[i] = string((const char *) buffer);
    }

    return array;
}


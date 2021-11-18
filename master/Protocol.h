#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

using namespace std;

#define TCP_BUFFER_SIZE 2000

class Protocol {
public:
    static void send(string * s, int size, int descriptor);
    static string * receive(int descriptor, int * size);

    static const unsigned int bufferSize;
    static void * buffer;
    
    static string to_string(int v);
};

#endif /* PROTOCOL_H */


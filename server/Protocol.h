#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

using namespace std;

#define TCP_BUFFER_SIZE 2000

class Protocol {
public:
    Protocol();
    ~Protocol();
    
    void send(string * s, int size, int descriptor);
    string * receive(int descriptor, int * size);

    void * buffer;
    
    static const unsigned int bufferSize;
    static string to_string(int v);
};

#endif /* PROTOCOL_H */


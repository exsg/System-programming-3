#ifndef SERVERCONF_H
#define SERVERCONF_H

#include <string>

using namespace std;


class ServerConfiguration {
public:
    static int statistics_fd;
    static int queries_fd;

};

#endif


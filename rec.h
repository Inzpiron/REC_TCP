#ifndef __REC_H_INCLUDED__
#define __REC_H_INCLUDED__

//http://www.inf.pucrs.br/~cnunes/redes_si/Sockets.htm

#include <iostream>
#include <bitset>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#include <thread>
#include <future>

typedef unsigned short int _bit;



namespace rec {
    const _bit BIT_ACK = 0x1;
    const _bit BIT_SYN = 0x2;
    const _bit BIT_FIN = 0x4;

    const _bit BIT_SVR = 0x1;
    const _bit BIT_CLI = 0x2;

    class TCPInter;
    class PackageInter;

    bool checkBit(_bit, _bit);
}

class rec::TCPInter {
private:
    std::future<void>  _tlisten;
    struct sockaddr_in _addr;
    int _socket;
    int _port;
    size_t buffer_size;

    void assert(_bit);
    void listen();
public:
    TCPInter(_bit, int, size_t);
    void start();
    void wait_close();
    void send_data();
};

class rec::PackageInter {
private:
    _bit bit;
    char * data;

public:
    PackageInter(_bit, char *);
};

#endif

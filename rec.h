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

typedef unsigned short int _bit;


bool checkBit(_bit, _bit);

namespace rec {
    const _bit BIT_ACK = 0x1;
    const _bit BIT_SYN = 0x2;
    const _bit BIT_FIN = 0x4;

    const _bit BIT_SRV = 0x1;
    const _bit BIT_CLI = 0x2;

    class TCPInter;
}

class rec::TCPInter {
private:
    struct sockaddr_in addr;
    struct hostent *h;
    int sd, rc, n, cliLen;
public:
    TCPInter(_bit b);
};

#endif

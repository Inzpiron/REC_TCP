#include "rec.h"
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
typedef unsigned short int _bit;
using namespace std;

bool checkBit(_bit b1, _bit b2) {
    if((b1 & b2) == b2) return true;

    return false;
}

rec::TCPInter::TCPInter(int port) {
    this->port = port;
    if(checkBit(b, rec::BIT_SVR)) {
        int rc, n, cliLen;
        if((_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            exit(0);
        }

        this->addr.sin_family      = AF_INET;
        this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->addr.sin_port        = htons(port);

        if((rc = bind(this->_socket, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
            exit(0);
        }
    }
}

void rec::TCPInter::listen() {
    while(1) {
        int n;
        char msg[1500];
        memset(msg, 0x0, 1500);
        struct sockaddr_in cliAddr;

        int serverLenght = sizeof(struct sockaddr_in);
        if((n = recvfrom(_socket, msg, 1500, 0, (struct sockaddr *) &cliAddr, (socklen_t*) &serverLenght) < 0) {
            continue;
        }

        printf("from %s:UDP%u : %s \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), msg);
    }
}

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

bool rec::checkBit(_bit b1, _bit b2) {
    if((b1 & b2) == b2) return true;

    return false;
}

rec::TCPInter::TCPInter(_bit b, int port) {
    if(checkBit(b, rec::BIT_SVR)) {
        if((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            exit(0);
        }

        this->_addr.sin_family      = AF_INET;
        this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->_addr.sin_port        = htons(port);

        int rc;
        if((rc = bind(this->_socket, (struct sockaddr *) &this->_addr, sizeof(this->_addr))) < 0) {
            exit(0);
        }
    }
}

void rec::TCPInter::listen() {
    while(1) {
        char msg[1500];

        /* init buffer */
        memset(msg, 0x0, 1500);
        sockaddr_in cliAddr;

        /* receive message */
        int serverLenght = sizeof(struct sockaddr_in);
        int n = recvfrom(this->_socket, msg, 1500, 0, (struct sockaddr *) &cliAddr, (socklen_t *) &serverLenght);

        if(n<0) {
            //printf("%s: cannot receive data \n",argv[0]);
            continue;
        }

        /* print received message */
        printf("from %s:UDP%u : %s \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), msg);

    }/* end of server infinite loop */
}

#include "rec.h"
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <future>
typedef unsigned short int _bit;
using namespace std;

bool rec::checkBit(_bit b1, _bit b2) {
    if((b1 & b2) == b2) return true;

    return false;
}

rec::TCPInter::TCPInter(_bit b, int port, size_t bs) {
    this->buffer_size = bs;

    if((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        exit(0);
    }

    if(checkBit(b, rec::BIT_SVR)) {
        this->_addr.sin_family      = AF_INET;
        this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->_addr.sin_port        = htons(port);

        int rc;
        if((rc = bind(this->_socket, (struct sockaddr *) &this->_addr, sizeof(this->_addr))) < 0) {
            exit(0);
        }
    }

    else if(checkBit(b, rec::BIT_CLI)) {
        struct hostent *h;
        h = gethostbyname(strdup("localhost"));
        if(h== NULL) {
            printf("unknown host");
            exit(1);
        }

        this->_addr.sin_family = h->h_addrtype;
        memcpy((char *) &this->_addr.sin_addr.s_addr,h->h_addr_list[0], h->h_length);
        this->_addr.sin_port = htons(port);
    }
}

void rec::TCPInter::start() {
    this->_tlisten = async(&rec::TCPInter::listen, this);
}

void rec::TCPInter::wait_close() {
    this->_tlisten.get();
}

void rec::TCPInter::listen() {
    while(1) {
        char msg[this->buffer_size];
        /* init buffer */
        memset(msg, 0x0, this->buffer_size);
        sockaddr_in cliAddr;

        /* receive message */
        int serverLenght = sizeof(struct sockaddr_in);
        int n = recvfrom(this->_socket, msg, this->buffer_size, 0, (struct sockaddr *) &cliAddr, (socklen_t *) &serverLenght);
        if(n < 0) {
            printf("Cannot receive data \n");
            continue;
        }
        /* print received message */
        printf("from %s:UDP%u : %s \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), msg);
    }/* end of server infinite loop */
}

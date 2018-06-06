#include "rec.h"
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

using namespace std;

checkBit(_bit b1, _bit b2) {
    if((b1 & b2) == b2) return true;

    return false;
}

rec::TCPInter::TCPInter(littleBit b, int port) {
    if(checkBit(b, rec::BIT_SVR)) {
        if((this->sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            exit(0);
        }

        this->addr.sin_family      = AF_INET;
        this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->addr.sin_port        = htons(1500);

        if((this->rc = bind(sd, (struct sockaddr *) &this->servAddr, sizeof(addr))) < 0) {
            exit(0);
        }
    } /*else if ((b & rec::BIT_CLI) == rec::BIT_CLI){


        if(argc<3) {
            printf("usage : %s <server> <data1> ... <dataN> \n", argv[0]);
            exit(1);
        }


        //get server IP address (no check if input is IP address or DNS name
        this->h = gethostbyname("localhost");
        if(this->h==NULL) {
            exit(1);
        }

        printf("%s: sending data to '%s' (IP : %s) \n", h->h_name, inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

        this->servAddr.sin_family = this->h->h_addrtype;
        memcpy((char *) &this->servAddr.sin_addr.s_addr, this->h->h_addr_list[0], this->h->h_length);
        servAddr.sin_port = htons(REMOTE_SERVER_PORT);

        // socket creation
        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sd<0) {
            printf("%s: cannot open socket \n",argv[0]);
            exit(1);
        }

        // bind any port
        cliAddr.sin_family      = AF_INET;
        cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        cliAddr.sin_port        = htons(0);

        rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
        if(rc<0) {
            printf("%s: cannot bind port\n", argv[0]);
            exit(1);
        }

    }
    */
}

#include "rec.h"
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <utility>
#include <queue>
typedef unsigned short int _bit;
using namespace std;

rec::PackageInter::PackageInter() {
    this->bit  = 0;
    this->data = NULL;
    this->_port   = -1;
    this->_last   = -1;
    this->_n_seq  = -1;
    this->_n_ack  = -1;
}

rec::PackageInter::PackageInter(size_t bs) {
    this->bit  = 0;
    this->data = new char[bs];
    this->_port   = -1;
    this->_last   = -1;
    this->_n_seq  = -1;
    this->_n_ack  = -1;
}

rec::PackageInter::PackageInter(_bit b, char * d, int nseq, int nack, bool last, size_t bs) {
    this->bit  = b;
    this->data   = new char[bs];

    if(d != NULL) {
        strcpy(d, this->data);
    }

    this->_port   = -1;
    this->_last   = last;
    this->_n_seq  = nseq;
    this->_n_ack  = nack;
}

_bit rec::PackageInter::get_bit(){
    return this->bit;
}

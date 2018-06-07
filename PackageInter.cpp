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

rec::PackageInter::PackageInter(_bit b, char * d) {
    this->bit  = b;
    this->data = d;
}

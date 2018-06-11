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

rec::pkg_addr::pkg_addr(rec::PackageInter _p, struct sockaddr_in _addr) {
    this->pkg  = _p;
    this->addr = _addr;
}

rec::pkg_addr::pkg_addr(void) {
    this->pkg = PackageInter();
}

#include <iostream>
#include "rec.h"
typedef unsigned short int _bit;
using namespace std;

int main() {
    rec::TCPInter serv(rec::BIT_SVR, 1500);

    serv.listen();
}

#include <iostream>
#include <string>
#include "rec.h"
typedef unsigned short int _bit;

using namespace std;

int main(int argc, char ** argv) {
    int porta = stoi(argv[1]);
    rec::TCPInter serv(rec::BIT_SVR, porta);

    serv.listen();
}

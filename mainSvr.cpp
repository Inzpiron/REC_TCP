#include <iostream>
#include <string>
#include "rec.h"
typedef unsigned short int _bit;

using namespace std;

int main(int argc, char ** argv) {
    //
    int porta = stoi(argv[1]);
    rec::TCPInter serv(rec::BIT_SVR, porta, 1500);

    serv.start();
    //serv.send_data((char*) "STRING DO SERVIDOR");
    //serv.send_data((char*) "STRING DO SERVIDOR2");
    //serv.send_data((char*) "STRING DO SERVIDOR3");
    serv.wait_close();
}

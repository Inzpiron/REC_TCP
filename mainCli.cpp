#include <iostream>
#include <string>
#include "rec.h"
typedef unsigned short int _bit;

using namespace std;

int main(int argc, char ** argv) {
    size_t bs = 1500;
    int svport = stoi(argv[1]);
    rec::TCPInter cli(rec::BIT_CLI, bs);

    cli.connect(svport, (char *) "localhost");
    cli.start();
    cli.send_data((char*) "Primeiro pacote");
    cli.send_data((char*) "Segundo pacote");
    cli.send_data((char*) "Terceiro pacote");
    cli.wait_close();
}

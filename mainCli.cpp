#include <iostream>
#include <string>
#include "rec.h"
typedef unsigned short int _bit;

using namespace std;

int main(int argc, char ** argv) {
    int svport = stoi(argv[1]);
    rec::TCPInter cli(rec::BIT_CLI, 5);

    cli.connect(svport, (char *) "localhost");
    cli.start();
    cli.send_data((char *) "leonardo valério anastacio, doidera mano");

    cli.disconnect();
    cli.wait_close();
}

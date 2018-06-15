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
    this_thread::sleep_for(chrono::milliseconds(4000));
    cli.send_data((char*) "PRIMEIRA BOSTA ENVIADA PELO CLIENTE");
    //this_thread::sleep_for(chrono::milliseconds(4000));
    cli.send_data((char*) "SEGUNDA BOSTA ENVIADA PELO CLIENTE");

    cli.wait_close();
}

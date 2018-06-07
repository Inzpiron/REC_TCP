#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct str{
    int a;
    int b;
    int c;
    long double d;
    char * msg;

    str(){
        a   = 1;
        b   = 2;
        c   = 3;
        d   = 0.0000099;
        msg = strdup("leonardo valerio anastacio");
    }
};

typedef struct str str;

int main() {
    void * bit = malloc(1);
    memcpy(bit, &(0x0), 1);
}

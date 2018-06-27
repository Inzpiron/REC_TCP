#ifndef __REC_H_INCLUDED__
#define __REC_H_INCLUDED__

//Auxilio -> http://www.inf.pucrs.br/~cnunes/redes_si/Sockets.htm

#include <iostream>
#include <bitset>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#include <thread>
#include <future>
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <utility>
#include <queue>
#include <fstream>

typedef unsigned short int _bit;

namespace rec {
    const _bit BIT_SYN     = 1;
    const _bit BIT_SYN_ACK = 2;
    const _bit BIT_FIN     = 4;
    const _bit BIT_FIN_ACK = 8;
    const _bit BIT_ACK     = 16;
    const _bit BIT_DTA     = 32;

    const _bit BIT_SVR = 1;
    const _bit BIT_CLI = 2;

    class PackageInter;
    class TCPInter;
    class pkg_addr;

    struct PkgInter;
}

struct rec::PkgInter {
    _bit bit;
    char _data[1500];
    int  _n_seq;
    int  _n_ack;
    bool _last;

    PkgInter();
    PkgInter(_bit, char *, int, int, bool);
};
typedef struct rec::PkgInter PkgInter;

class rec::TCPInter {
private:
    std::ofstream log_file;
    size_t buffer_size;
    std::map<int, std::pair<PkgInter, sockaddr_in> >  entry_buffer;
    std::queue < std::pair<PkgInter, sockaddr_in>  >  sender_buffer;
    std::queue < PkgInter > read_buffer;

    std::future<void> _thread_listen;
    std::future<void> _thread_sender_buffer;

    std::mutex mtx_entry_buffer;
    std::mutex mtx_send_buffer;
    std::mutex mtx_read_buffer;

    std::map<int, bool> _pkg_received;

    struct sockaddr_in _my_addr;
    struct sockaddr_in _other_addr;  //CLIENT
    int _socket;


    int _w_size = 2;

    bool _online = true;
    bool _flag_first;
    int _n_seq;
    int _n_seq_send = -1;
    int _n_seq_ack;


    int _p_listen;

    _bit entity;

    bool _hand_shaked;

    void assert(PkgInter&, sockaddr_in &);
    void listen();
    void check_entry_buffer();
    void check_sender_buffer();
    void send(PkgInter&, sockaddr_in &);
public:
    TCPInter();
    TCPInter(_bit, int, size_t); //SERVER
    TCPInter(_bit, size_t); //CLIENT

    void connect(int, char *); //CLIENT

    void send_data(char *); //CLIENT

    char * listen_data();

    void disconnect();
    void start();
    void wait_close();
    void send_data();
};

#endif

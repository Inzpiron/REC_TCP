#ifndef __REC_H_INCLUDED__
#define __REC_H_INCLUDED__

//http://www.inf.pucrs.br/~cnunes/redes_si/Sockets.htm

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

typedef unsigned short int _bit;

namespace rec {
    const _bit BIT_SYN     = 0x1;
    const _bit BIT_SYN_ACK = 0x2;
    const _bit BIT_FIN     = 0x4;
    const _bit BIT_FIN_ACK = 0x8;
    const _bit BIT_ACK     = 0x16;
    const _bit BIT_DTA     = 0x32;

    const _bit BIT_SVR = 0x1;
    const _bit BIT_CLI = 0x2;

    class PackageInter;
    class TCPInter;
    class pkg_addr;

    bool checkBit(_bit, _bit);
}

class rec::PackageInter {
private:
    _bit bit;
    char * data;

public:
    PackageInter(_bit, char *, int, int, bool);
    _bit  get_bit(void);
    unsigned  _port;
    int _cli_id;
    int _n_seq;
    int _n_ack;
    bool _last;
};

class rec::pkg_addr {
public:
    pkg_addr(PackageInter&, sockaddr_in);
    PackageInter pkg;
    sockaddr_in addr;
};

class rec::TCPInter {
private:
    size_t buffer_size;
    std::map<int, pkg_addr >  entry_buffer;
    std::queue < pkg_addr >  sender_buffer;

    std::future<void> _thread_listen;
    std::future<void> _thread_sender_buffer;

    std::mutex mtx_entry_buffer;
    std::mutex mtx_send_buffer;
    std::vector<unsigned>  _cli_port_list;
    std::map<int, bool> _pkg_received;

    struct sockaddr_in _addr;
    struct sockaddr_in _sv_addr;  //CLIENT
    int _socket;
    unsigned _port;

    int _n_seq;
    int _n_ack;


    unsigned _cli_id; //CLIENT
    unsigned _cli_id_sum;

    _bit entity;

    void assert(PackageInter *, sockaddr_in &);
    void listen();
    void check_entry_buffer();
    void check_sender_buffer();
    void sendd(PackageInter *, sockaddr_in &);
public:
    TCPInter(_bit, int, size_t); //SERVER
    TCPInter(_bit, size_t); //CLIENT

    void connect(int, char *); //CLIENT

    void start();
    void wait_close();
    void send_data();
};

#endif

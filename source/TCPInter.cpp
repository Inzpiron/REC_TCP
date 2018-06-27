#include "rec.h"
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <future>
#include <string>
#include <utility>
#include <queue>
#include <chrono>
#include <cmath>
#include <fstream>
typedef unsigned short int _bit;
using namespace std;

rec::PkgInter::PkgInter(_bit b, char * d, int s, int a, bool f) {
    this->bit   = b;
    if(d != NULL) {
        strcpy(this->_data, d);
    }
    this->_n_seq = s;
    this->_n_ack = a;
    this->_last  = f;
}

rec::PkgInter::PkgInter() {
    *this = *(new PkgInter(0, NULL, -2, -2, true));
}

/*
TCPInter() (para servidor)
Parametros: entidade (define se é cliente ou servidor), porta do servidor,
            tamanho do pacote (MTU)
Objetivo: Inicializar o servidor
*/
rec::TCPInter::TCPInter(_bit b, int port, size_t bs) {
    this->_hand_shaked = false;

    if((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        exit(0);
    }
    this->buffer_size = bs;
    this->entity = b;
    this->_n_seq = 0;

    if(b==rec::BIT_SVR) {
        if((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            exit(0);
        }

        this->_my_addr.sin_family      = AF_INET;
        this->_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->_my_addr.sin_port        = htons(port);

        int rc;
        if((rc = bind(this->_socket, (struct sockaddr *) &this->_my_addr, sizeof(this->_my_addr))) < 0) {
            exit(0);
        }
    } else
        exit(0);
}

/*
TCPInter() (para cliente)
Parametros: entidade (define se é cliente ou servidor), tamanho do pacote (MTU)
Objetivo: Inicializar o cliente
*/
rec::TCPInter::TCPInter(_bit b, size_t bs) {
    this->_hand_shaked = false;

    if((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        exit(0);
    }
    this->buffer_size = bs;
    this->entity = b;
    this->_n_seq = 0;

    if(b== rec::BIT_CLI) {
        int rc = -1;
        do {
            this->_my_addr.sin_family      = AF_INET;
            this->_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            srand(time(NULL));
            this->_my_addr.sin_port        = htons(rand()%1000 + 4000);
            rc = bind(this->_socket, (struct sockaddr *) &this->_my_addr, sizeof(this->_my_addr));
        } while(rc < 0);

        getsockname(this->_socket, (struct sockaddr*) &this->_my_addr, (socklen_t *) sizeof(this->_my_addr));
        printf("The selected port is %d.\n\n", ntohs(this->_my_addr.sin_port));
    } else {
        exit(0);
    }
}

/*
start();
Objetivo: Inicialização.
*/
void rec::TCPInter::start() {
    log_file = ofstream(this->entity == rec::BIT_CLI ? "log-cli.txt" : "log-svr.txt");
    this->_n_seq_send = 0;
    this->_flag_first = false;
    (this->_pkg_received)[-1] = true;

    this->_thread_listen = async(std::launch::async, &rec::TCPInter::listen, this);
    this->_thread_sender_buffer = async(std::launch::async, &rec::TCPInter::check_sender_buffer, this);
    while(!this->_hand_shaked){

    }
}

/*
wait_close:
Objetivo: Esperar todas as threads terminarem
*/
void rec::TCPInter::wait_close() {
    this->_thread_sender_buffer.get();
    this->_thread_listen.get();
}

/*
connect():
Objetivo: Fazer a conexão do cliente com o servidor
*/
void rec::TCPInter::connect(int svport, char * svaddr) {
    if(this->entity == rec::BIT_CLI) {
        struct hostent *h;
        h = gethostbyname(svaddr);
        if(h== NULL) {
            printf("unknown host");
            exit(1);
        }

        this->_other_addr.sin_family = h->h_addrtype;
        memcpy((char *) &this->_other_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
        this->_other_addr.sin_port = htons(svport);

        PkgInter p(BIT_SYN, NULL, this->_n_seq, -1, true);

        cout << "<-sending SYN to server" << endl <<
                          "<-PKG sending: BIT = " << p.bit << " _n_seq = " << p._n_seq << " _n_ack = " << p._n_ack << endl << endl;
        mtx_send_buffer.lock();
        this->sender_buffer.push({p, this->_other_addr});
        mtx_send_buffer.unlock();

        /*
        if(sendto(this->_socket, (void *) &p, sizeof(PackageInter), 0, (struct sockaddr *) &this->_other_addr, sizeof(this->_other_addr)) < 0) {
            close(this->_socket);
            exit(0);
        }*/


    } else exit(0);
}

/*
check_sender_buffer():
Objetivo: Controle do buffer de saída, todo pacote enviado, exceto acks,
          passarão pelo buffer de saída. Ele é responsável pelo re-envio de
          pacotes até que receba um Ack.
*/
void rec::TCPInter::check_sender_buffer() {
    vector<PkgInter> window_buff;

    while(true) {
        //if(this->_hand_shaked == false)
        //    cout << this->_n_seq << endl;
        if(!this->sender_buffer.empty()) {
            PkgInter p_        = this->sender_buffer.front().first;
            sockaddr_in  addr_ = this->sender_buffer.front().second;

            this->send(p_, addr_);
            this_thread::sleep_for(chrono::milliseconds(250));

            if(this->_pkg_received[p_._n_seq] || p_.bit == rec::BIT_ACK) {
                if(p_.bit == rec::BIT_FIN_ACK)
                    this->_online = false;

                mtx_send_buffer.lock();
                this->sender_buffer.pop();
                mtx_send_buffer.unlock();
            } else {
                cout << "<#Sending pkg again" << endl;
            }
        } else {
            if(!this->_online) {
                return;
            }
        }
    }
}

/*
listen():
Objetivo: listen irá ficar em loop infinito, sua função é ler pacotes em um
          socket de entrada. Ao receber um pacote ele irá repassar para
          assert(), que definirá o que deve ser feito.
*/
void rec::TCPInter::listen() {
    while(this->_online) {
        PkgInter * p = new PkgInter(0, NULL, -2, -2, true);
        sockaddr_in senderAddr;

        int serverLenght = sizeof(struct sockaddr_in);
        int n = recvfrom(this->_socket, p, sizeof(PkgInter), 0, (struct sockaddr *) &senderAddr, (socklen_t *) &serverLenght);

        if(n < 0) {
            continue;
        }

        cout << "->FROM PORT " << ntohs(senderAddr.sin_port) << endl <<
                "->PKG received: BIT = " << (*p).bit << ", _n_seq = " << (*p)._n_seq << ", _n_ack = " << (*p)._n_ack << endl;
        this->assert(*p, senderAddr);
    }
}

/*
assert():
parametro: pacote e informação do endereço remetente
objetivo: Responsável pelo fluxo de controle, quando um pacote é recebido pela
          aplicação está função define o que será feito. Por exemplo, se receber
          um pacote "SYN" será enviado "SYN_ACK" pro remetente.
*/
void rec::TCPInter::assert(rec::PkgInter& p, sockaddr_in & senderAddr) {
    (this->entry_buffer) [p._n_seq] = {p, senderAddr};
    (this->_pkg_received)[p._n_ack] = true;

    if(p.bit == rec::BIT_SYN) {
        this->_n_seq     = 0;
        this->_n_seq_ack = 0;
        this->_other_addr = senderAddr;

        PkgInter p_to_send(rec::BIT_SYN_ACK, NULL, this->_n_seq, 0, true);

        mtx_send_buffer.lock();
        this->sender_buffer.push({p_to_send, this->_other_addr});
        mtx_send_buffer.unlock();

        cout << "->SYN: From " << inet_ntoa(this->_other_addr.sin_addr) << endl <<
                          "<-sending SYN_ACK to client" << endl <<
                          "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;

    }

    else if(rec::BIT_SYN_ACK == p.bit){
        this->_n_seq     = 1;
        this->_n_seq_ack = 0;
        PkgInter p_to_send(rec::BIT_ACK, NULL, -1, p._n_seq, true);

        mtx_send_buffer.lock();
        this->sender_buffer.push({p_to_send, this->_other_addr});
        mtx_send_buffer.unlock();

        cout << "->SYN_ACK: From " << inet_ntoa(senderAddr.sin_addr) << endl
                       << "<-sending ACK to server" << endl
                       << "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;

        this->_hand_shaked = true;
    }

    else if(rec::BIT_ACK == p.bit) {
        if(p._n_ack == this->_n_seq) {
            this->_n_seq++;
        }

        if(this->entity == rec::BIT_SVR && p._n_ack == 0)
            this->_hand_shaked = true;

        cout << "->ACK: From " << inet_ntoa(senderAddr.sin_addr) << endl << endl;
    }

    else if(rec::BIT_DTA == p.bit) {
        mtx_read_buffer.lock();
        read_buffer.push(p);
        mtx_read_buffer.unlock();

        PkgInter p_to_send(rec::BIT_ACK, NULL, -1, p._n_seq, true);
        //this->sender_buffer.push({p_to_send, this->_other_addr});
        this->send(p_to_send, this->_other_addr);
        cout << "->DTA: From " << inet_ntoa(senderAddr.sin_addr) << endl
                       << "->" << (p)._data << endl
                       << "<-sending ACK to remetent" << endl
                       << "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;
    }

    else if(rec::BIT_FIN == p.bit) {
        PkgInter p_to_send(rec::BIT_FIN_ACK, NULL, this->_n_seq, p._n_seq, true);

        mtx_send_buffer.lock();
        this->sender_buffer.push({p_to_send, this->_other_addr});
        mtx_send_buffer.unlock();

        cout << "->FIN: From " << inet_ntoa(senderAddr.sin_addr) << endl
                       << "<-sending FIN_ACK to remetent" << endl
                       << "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;
    }

    else if(rec::BIT_FIN_ACK == p.bit) {
        PkgInter p_to_send(rec::BIT_ACK, NULL, -1, p._n_seq, true);
        this->send(p_to_send, this->_other_addr);

        cout << "->FIN_ACK: From " << inet_ntoa(senderAddr.sin_addr) << endl
                       << "<-sending ACK to remetent" << endl
                       << "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;
        this->_online = false;
    }
}

/*
send_data():
parametro: informção a ser enviada
objetivo: Enviar pacotes com informação contido no ponteiro de char "_data",
          caso tamanho da informação for maior que o buffer_size definido então
          os pacotes serão fragmentados.
*/
void rec::TCPInter::send_data(char * _data) {
    if(this->_hand_shaked == false) {
        cout << "Handshake não executado" << endl;
        exit(0);
    } else {
        if(this->_n_seq_send == -1) this->_n_seq_send = this->_n_seq;

        int n_it = ceil(strlen(_data)/((double) this->buffer_size));

        for(int i = 0; i < n_it; i++) {
            char * dat = (char *) malloc(sizeof(char) * 1500);
            strncpy(dat, _data + i*this->buffer_size, this->buffer_size);
            dat[(i+1)*this->buffer_size] = '\0';

            PkgInter p_to_send(rec::BIT_DTA, dat, this->_n_seq_send, -1, (i == n_it - 1) ? true : false);
            cout << "<-sending DTA to " << inet_ntoa(this->_other_addr.sin_addr) << " : " << ntohs(this->_other_addr.sin_port) << endl
                 << "<-PKG sending: BIT = " << p_to_send.bit << " _n_seq = " << p_to_send._n_seq << " _n_ack = " << p_to_send._n_ack << endl << endl;

            mtx_send_buffer.lock();
            this->sender_buffer.push({p_to_send, this->_other_addr});
            mtx_send_buffer.unlock();
            this->_n_seq_send++;
        }
    }
}

/*
listen_data():
objetivo: Desfragmentar e retornar os dados contido nos pacotes do buffer de
          entrada.
*/
char * rec::TCPInter::listen_data() {
    bool check = false;
    char * data = (char *) malloc(sizeof(1500));
    while(true) {
        if(!read_buffer.empty()) {
            PkgInter p = read_buffer.front();
            strcat(data, p._data);

            mtx_read_buffer.lock();
            this->read_buffer.pop();
            mtx_read_buffer.unlock();

            if(p._last == true)
                break;
        }
    }

    return data;
}

/*
disconnect():
objetivo: Disparar pacote "FIN" para o servidor
*/
void rec::TCPInter::disconnect() {
    if(this->entity == rec::BIT_CLI) {
        PkgInter p_to_send(rec::BIT_FIN, NULL, this->_n_seq, -1, true);

        mtx_send_buffer.lock();
        this->sender_buffer.push({p_to_send, this->_other_addr});
        mtx_send_buffer.unlock();
    } else {
        this->_online = false;
    }
    //cout.close();
}

/*
send():
Parâmetros: Pacote a ser enviado, destinatário.
Objetivo: Enviar pacotes.
*/
void rec::TCPInter::send(PkgInter& p, sockaddr_in& recipient) {
    if(sendto(this->_socket, &p, sizeof(PkgInter), 0, (struct sockaddr *) &recipient, sizeof(recipient)) < 0) {
        close(this->_socket);
        exit(0);
    }
}

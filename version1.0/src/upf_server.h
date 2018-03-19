#ifndef UPF_SERVER_H
#define UPF_SERVER_H

#include "../../NFV_LTE_EPC-1.0/src/diameter.h"
#include "../../NFV_LTE_EPC-1.0/src/gtp.h"
#include "../../NFV_LTE_EPC-1.0/src/network.h"
#include "../../NFV_LTE_EPC-1.0/src/packet.h"
#include "../../NFV_LTE_EPC-1.0/src/s1ap.h"
#include "../../NFV_LTE_EPC-1.0/src/upf.h"
#include "../../NFV_LTE_EPC-1.0/src/sync.h"
#include "../../NFV_LTE_EPC-1.0/src/udp_client.h"
#include "../../NFV_LTE_EPC-1.0/src/udp_server.h"
#include "../../NFV_LTE_EPC-1.0/src/utils.h"
//extern Upf g_upf;
extern int g_s11_server_threads_count;
extern int g_s1_server_threads_count;
//extern int g_s5_server_threads_count;
extern vector<thread> g_s11_server_threads;
extern vector<thread> g_s1_server_threads;
//extern vector<thread> g_s5_server_threads;
//extern Upf g_upf;
extern Upf g_upf;
void check_usage(int);
void init(char**);
void run();
void handle_s11_traffic();
void handle_s1_traffic();
//void handle_s5_traffic();
void handle_sgi_traffic();

#endif /* SGW_SERVER_H */

#ifndef AUSF_SERVER_H
#define AUSF_SERVER_H

#include "../../NFV_LTE_EPC-1.0/src/diameter.h"
#include "../../NFV_LTE_EPC-1.0/src/gtp.h"
#include "../../NFV_LTE_EPC-1.0/src/ausf.h"
#include "../../NFV_LTE_EPC-1.0/src/mysql.h"
#include "../../NFV_LTE_EPC-1.0/src/network.h"
#include "../../NFV_LTE_EPC-1.0/src/packet.h"
#include "../../NFV_LTE_EPC-1.0/src/s1ap.h"
#include "../../NFV_LTE_EPC-1.0/src/sctp_server.h"
#include "../../NFV_LTE_EPC-1.0/src/sync.h"
#include "../../NFV_LTE_EPC-1.0/src/utils.h"

extern Ausf g_ausf;
extern int g_workers_count;

void check_usage(int);
void init(char**);
void run();
int handle_mme(int, int);
void finish();

#endif //HSS_SERVER_H

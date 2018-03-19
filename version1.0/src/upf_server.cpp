#include "../../NFV_LTE_EPC-1.0/src/upf_server.h"
//#include "../../NFV_LTE_EPC-1.0/src/upf.h"

int g_s11_server_threads_count;
int g_s1_server_threads_count;
vector<thread> g_s11_server_threads;
vector<thread> g_s1_server_threads;

Upf g_upf;

void check_usage(int argc) {
	if (argc < 4) {
		TRACE(cout << "Usage: ./<sgw_server_exec> S11_SERVER_THREADS_COUNT S1_SERVER_THREADS_COUNT S5_SERVER_THREADS_COUNT" << endl;)
		g_utils.handle_type1_error(-1, "Invalid usage error: sgwserver_checkusage");
	}
}

void init(char *argv[]) {
	g_s11_server_threads_count = atoi(argv[1]);
	g_s1_server_threads_count = atoi(argv[2]);
	
	g_s11_server_threads.resize(g_s11_server_threads_count);
	g_s1_server_threads.resize(g_s1_server_threads_count);
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;

	/* SGW S11 server */
	TRACE(cout << "SGW S11 server started" << endl;)
	g_upf.s11_server.run(g_upf_s11_ip_addr, g_upf_s11_port);
	for (i = 0; i < g_s11_server_threads_count; i++) {
		g_s11_server_threads[i] = thread(handle_s11_traffic);
	}	

	/* SGW S1 server */
	TRACE(cout << "SGW S1 server started" << endl;)
	g_upf.s1_server.run(g_upf_s1_ip_addr, g_upf_s1_port);
	for (i = 0; i < g_s1_server_threads_count; i++) {
		g_s1_server_threads[i] = thread(handle_s1_traffic);
	}


	/* Joining all threads */
	for (i = 0; i < g_s11_server_threads_count; i++) {
		if (g_s11_server_threads[i].joinable()) {
			g_s11_server_threads[i].join();
		}
	}
	for (i = 0; i < g_s1_server_threads_count; i++) {
		if (g_s1_server_threads[i].joinable()) {
			g_s1_server_threads[i].join();
		}
	}	
					
}

void handle_s11_traffic() {
	UdpClient upf_client;
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	upf_client.set_client(g_upf_s1_ip_addr);
	while (1) {
		g_upf.s11_server.rcv(src_sock_addr, pkt);
		pkt.extract_gtp_hdr();
		switch(pkt.gtp_hdr.msg_type) {
			/* Create session */
			case 1:
				TRACE(cout << "sgwserver_handles11traffic:" << " case 1: create session" << endl;)
				g_upf.handle_create_session(src_sock_addr, pkt, upf_client);
				break;

			/* Modify bearer */
			case 2:
				TRACE(cout << "sgwserver_handles11traffic:" << " case 2: modify bearer" << endl;)
				g_upf.handle_modify_bearer(src_sock_addr, pkt);
				break;

			/* Detach */
			case 3:
				TRACE(cout << "sgwserver_handles11traffic:" << " case 3: detach" << endl;)
				g_upf.handle_detach(src_sock_addr, pkt, upf_client);
				break;
			/* Indirect tunnel */
			case 4:
				cout << "handle indirec tunnel setup:" << " case 4: handle_indirect_tunnel_setup" << endl;
				g_upf.handle_indirect_tunnel_setup(src_sock_addr, pkt);
				break;
			case 5:
				cout << "switch downlink tunnel id to target ran:" << " case 5: handle completion mark" << endl;
				g_upf.handle_handover_completion(src_sock_addr, pkt);
				break;
			case 6:
				cout << "remove the uplink indirect tunnel id" << " case 6: handle tear down" << endl;
				g_upf.handle_indirect_tunnel_teardown_(src_sock_addr, pkt);
			break;
			/* For error handling */
			default:
				TRACE(cout << "sgwserver_handles11traffic:" << " default case:" << endl;)
		}		
	}
}

void handle_s1_traffic() {
	UdpClient upf_client;
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	upf_client.set_client(g_upf_s1_ip_addr);   // Change in header file SGW
	while (1) {
		g_upf.s1_server.rcv(src_sock_addr, pkt);
		pkt.extract_gtp_hdr();
		switch(pkt.gtp_hdr.msg_type) {
			/* Uplink userplane data */
			case 1:
				TRACE(cout << "sgwserver_handles1traffic:" << " case 1: uplink udata" << endl;)
				g_upf.handle_uplink_udata(pkt, upf_client);  //upf_client    pgw_s5_client
				break;

			/* For error handling */
			default:
				TRACE(cout << "sgwserver_handles1traffic:" << " default case:" << endl;)
		}		
	}		
}



int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}

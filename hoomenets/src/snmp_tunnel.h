#ifndef __SNMP_TUNNEL_H__
#define __SNMP_TUNNEL_H__

#define DEFAULT_SNMP_MANAGER_HOST "thetida.stanford.edu"
#define DEFAULT_SNMP_MANAGER_PORT 6693
#define DEFAULT_SNMPD_PORT 161
#define DEFAULT_SNMPTD_PORT 1162

#define DEFAULT_KEEP_ALIVE_INTERVAL 60

#define MAX_MSG_SIZE 1500
#define SLEEP_MAX 60
#define MAX_SNMP_CLIENTS 10
#define MAX_SNMP_MANAGERS 32

#define MANAGER_TIMEOUT 10 // secs
#define AGENT_TIMEOUT 300 // secs
#define AGENT_KEEPALIVE 60000 // msecs

#define SNMP_TYPE 1
#define ECHO_REQUEST 2
#define ECHO_REPLY 3
#define SNMP_TRAP_TYPE 4

#define SUCCESS 0
#define FAILURE -1
#define true 1
#define false 0

typedef struct {
	uint16_t type;
	uint16_t len;
	uint32_t hash;
} tunnel_pkt_header;

typedef struct {
	tunnel_pkt_header header;
	char data[];
} snmp_packet;

/* starts the tunnel between the client and the local SNMP manager */
void snmp_start_tunnel(int sock);

/* connects to the SNMP manager */
int snmp_tunnel_connect(struct sockaddr_in *);

/* reconnects to the SNMP manager */
int snmp_tunnel_reconnect(int, struct sockaddr_in *);

#endif

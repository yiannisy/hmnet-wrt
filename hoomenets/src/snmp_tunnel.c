#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
//#include <linux/if.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>
//#include <sys/stropts.h>

#include "snmp_tunnel.h"

#define MAX_HOSTNAME_SIZE 120
#define SUCCESS 0

static bool is_client = false;
static bool is_server = false;
static uint16_t keep_alive_interval = DEFAULT_KEEP_ALIVE_INTERVAL;
static uint16_t snmp_manager_port = DEFAULT_SNMP_MANAGER_PORT;
static char * snmp_manager_host = NULL;
static uint16_t snmpd_port = DEFAULT_SNMPD_PORT;
static uint16_t snmptd_port = DEFAULT_SNMPTD_PORT;
static struct sockaddr_in snmp_manager_addr;

/* each snmp manager is represented through this struct.
 * each packet coming from the manager will have the hash
 * on its header.
 */
struct snmp_manager {
	bool is_active;
	struct pollfd * pfd;
	uint32_t hash;
	int last_active;
};


/* Translates 'host_name', which may be a DNS name or an IP address, into a
 * numeric IP address in '*addr'.  Returns 0 if successful, otherwise a
 * positive errno value. */
static int
lookup_ip(const char *host_name, struct in_addr *addr)
{
    if (!inet_aton(host_name, addr)) {
        struct hostent *he = gethostbyname(host_name);
        if (he == NULL) {
            return ENOENT;
        }
        addr->s_addr = *(uint32_t *) he->h_addr;
    }
    return 0;
}

static int
second_now(){
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec;
}

static struct snmp_manager *
demultiplex_manager(struct snmp_manager * managers, tunnel_pkt_header * hdr){
	int i;
	for (i = 0 ; i < MAX_SNMP_MANAGERS ; i++ ){
		if (ntohl(hdr->hash) == managers[i].hash) {
			return &managers[i];
		}
	}
	// manager not found - pick up an unused bucket.
	for (i = 0; i < MAX_SNMP_MANAGERS ; i++){
		if ( (second_now() - managers[i].last_active) > MANAGER_TIMEOUT){
			managers[i].hash = ntohl(hdr->hash);
			managers[i].last_active = second_now();
			return &managers[i];
		}
	}

	// manager not found and all buckets are used...
	return NULL;
}

static int
send_echo_request(int tunnel_fd){
	tunnel_pkt_header pkt;

	pkt.type = ntohs(ECHO_REQUEST);
	pkt.len = ntohs(sizeof(pkt));
	pkt.hash = 0;

	int n_write = write(tunnel_fd, &pkt, sizeof(pkt));

	if(n_write != sizeof(pkt)) {
		printf("failed to send echo request...\n");
		return FAILURE;
	}
	else {
		return SUCCESS;
	}
}

static int
forward_tunnel_packet(struct snmp_manager manager, int tunnel_fd){
	int n_read, n_write;
	char buffer[MAX_MSG_SIZE];
	char * pkt;


	n_read = recvfrom(manager.pfd->fd, buffer, MAX_MSG_SIZE, 0, NULL, NULL);
	if(n_read < 0) {
		printf("cannot read from socket - reconnecting...(%s)\n",strerror(errno));
		return FAILURE;
	}
	uint16_t len = n_read + sizeof(tunnel_pkt_header);
	pkt = malloc(len);
	snmp_packet * snmp_pkt = (snmp_packet *) pkt;
	snmp_pkt->header.type = htons(SNMP_TYPE);
	snmp_pkt->header.len = htons(len);
	snmp_pkt->header.hash = htonl(manager.hash);
	memcpy(&snmp_pkt->data, buffer, n_read);

	n_write = write(tunnel_fd, pkt, len);
	free(pkt);
	if(n_write != len){
		printf("write failed (should write %d but wrote %d) (%s)\n",len, n_write, strerror(errno));
		return FAILURE;
	}
	else{
		return SUCCESS;
	}
}

static int
forward_tunnel_trap_packet(int trap_fd, int tunnel_fd){
	int n_read, n_write;
	char buffer[MAX_MSG_SIZE];
	char * pkt;


	n_read = recvfrom(trap_fd, buffer, MAX_MSG_SIZE, 0, NULL, NULL);
	if(n_read < 0) {
		printf("cannot read from trap socket - reconnecting...(%s)\n",strerror(errno));
		return FAILURE;
	}
	uint16_t len = n_read + sizeof(tunnel_pkt_header);
	pkt = malloc(len);
	snmp_packet * snmp_pkt = (snmp_packet *) pkt;
	snmp_pkt->header.type = htons(SNMP_TRAP_TYPE);
	snmp_pkt->header.len = htons(len);
	snmp_pkt->header.hash = 0; // not needed here...?
	memcpy(&snmp_pkt->data, buffer, n_read);

	n_write = write(tunnel_fd, pkt, len);
	free(pkt);
	if(n_write != len){
		printf("write failed (should write %d but wrote %d) (%s)\n",len, n_write, strerror(errno));
		return FAILURE;
	}
	else{
		return SUCCESS;
	}
}

static int
process_tunnel_packet(struct snmp_manager * managers, void * buffer, struct sockaddr_in snmpd_addr){
	int retval;
	tunnel_pkt_header * hdr = (tunnel_pkt_header *) buffer;
	uint16_t type = ntohs(hdr->type);
	struct snmp_manager * current_manager;
	switch(type) {
	case SNMP_TYPE:
		/* we need to find what's the manager sending this request. */
		current_manager = demultiplex_manager(managers, hdr);
		if (current_manager) {
			/* update the last_active value and forward the payload to
			 * the daemon.
			 */
			current_manager->last_active = second_now();
			snmp_packet * pkt = (snmp_packet *) buffer;
			uint16_t payload_len = ntohs(hdr->len) - sizeof(snmp_packet);
			int n_write = sendto(current_manager->pfd->fd, pkt->data, payload_len,
					0, (struct sockaddr*)&snmpd_addr, sizeof(snmpd_addr));
			if(n_write != payload_len){
				printf("write failed (should write %d but wrote %d) (%s)\n",
						payload_len, n_write, strerror(errno));
				retval = SUCCESS;
			}
			else{
				retval = SUCCESS;
			}

		}
		else {
			printf("too many active managers - dropping request...\n");
			retval =  FAILURE;
		}
		break;
	case ECHO_REQUEST:
		printf("received ECHO_REQUEST\n");
		retval = SUCCESS;
		break;
	case ECHO_REPLY:
		printf("received ECHO_REPLY\n");
		retval = SUCCESS;
		break;
	default:
		printf("received UNKNOWN packet (%d)\n", type);
		retval = FAILURE;
		break;
		}

	return retval;

}

int
snmp_tunnel_ap_connect(struct sockaddr_in *snmp_manager)
{
	int sleep_interval = 1;
	int snmp_sock;

	if((snmp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("Can't create SNMP tunnel socket\n");
		exit(1);
	}

	while(connect(snmp_sock, (struct sockaddr *)snmp_manager,sizeof(struct sockaddr)) < 0){
		printf("Could not connect to snmp manager - retry after %d seconds\n",sleep_interval);
		sleep(sleep_interval);
		sleep_interval *= 2;
		sleep_interval = sleep_interval > SLEEP_MAX ? SLEEP_MAX : sleep_interval;
	}
	printf("Connected to SNMP manager\n");
	return snmp_sock;
}

int
snmp_trap_setup()
{
	int snmptrap_fd;
	struct sockaddr_in snmp_trapd;

	bzero((char*)&snmp_trapd,sizeof(snmp_trapd));
	snmp_trapd.sin_family = AF_INET;
	snmp_trapd.sin_addr.s_addr = htonl(INADDR_ANY);
	snmp_trapd.sin_port = htons(snmptd_port);

	snmptrap_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	int optval = 1;
	if(setsockopt(snmptrap_fd,SOL_SOCKET,SO_REUSEADDR, (char *)&optval,sizeof(optval)) == -1) {
		printf("cannot set so_reuseaddr option\n");
		close(snmptrap_fd);
		return FAILURE;
	}
	if (snmptrap_fd == -1) {
		printf("failed to get socket for snmp traps...\n");
		return FAILURE;
	}
	if (bind(snmptrap_fd,(struct sockaddr*)&snmp_trapd,sizeof(snmp_trapd)) == -1){
		printf("failed to bind snmp trap socket...\n");
		return FAILURE;
	}
	return snmptrap_fd;
}

void
snmp_tunnel_ap_run(int tunnelfd,int trapfd)
{
	struct pollfd fds[MAX_SNMP_MANAGERS + 2];
	int num_fds = MAX_SNMP_MANAGERS + 2;
	char buffer[MAX_MSG_SIZE];
	struct sockaddr_in snmpd_addr;
	int n_read,n_write;
	struct snmp_manager managers[MAX_SNMP_MANAGERS];


	/* this is where the local snmpd agent listens */
	bzero((char*)&snmpd_addr, sizeof(snmpd_addr));
	snmpd_addr.sin_family = AF_INET;
	/* TODO : better define localhost here...? */
	snmpd_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	snmpd_addr.sin_port = htons(snmpd_port);

	/* prepare the sockets to be polled */
	/* tunnel-side */
	fds[0].fd = tunnelfd;
	fds[0].events = POLLIN | POLLHUP;


	/* prepare the sockets to be polled */
	/* tunnel-side */
	fds[1].fd = trapfd;
	fds[1].events = POLLIN | POLLHUP;




	/* initialize all manager structs. */
	int i;
	for (i = 0; i < MAX_SNMP_MANAGERS ; i++){
		managers[i].is_active = false;
		managers[i].pfd = &fds[i+2];
		managers[i].pfd->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		managers[i].pfd->events = POLLIN | POLLHUP;
		managers[i].last_active = 0;
	}

	int retval;
	int current_fd;
	tunnel_pkt_header * hdr;
	uint16_t type;
	int last_active = second_now();

	for(;;){

		retval = poll(fds, num_fds, AGENT_KEEPALIVE);

		if (retval == 0){
			printf("sending ECHO_REQUEST\n");
			send_echo_request(fds[0].fd);
		}

		else{
			/* if something came from the tunnel, send it to snmpd */
			if(fds[0].revents & POLLIN) {
				last_active = second_now();
				memset(buffer,'\0', MAX_MSG_SIZE);
				n_read = recv(fds[0].fd, buffer, MAX_MSG_SIZE, 0);
				if(n_read <= 0){
					printf("cannot read from socket - reconnecting...(%s)\n",strerror(errno));
					break;
				}
				else {
					if(process_tunnel_packet(managers, buffer, snmpd_addr) != SUCCESS) {
						printf("failed to process packet from tunnel...");
						// break; - no need to disconnect here...
					}
				}
			}
			
			/* forward snmp traps to the tunnel */
			if(fds[1].revents & POLLIN) {
				if(forward_tunnel_trap_packet(fds[1].fd,fds[0].fd) != SUCCESS) {
					printf("Failed to forward trap to tunnel...\n");
					break;
				}
			}


			/* if something came from snmpd, we need to multiplex and send to the tunnel */
			for (i = 0; i < MAX_SNMP_MANAGERS ; i++) {
				if (managers[i].pfd->revents & POLLIN) {
					if(forward_tunnel_packet(managers[i],fds[0].fd) != SUCCESS) {
						printf("failed to forward to tunnel...\n");
						break;
					}
				}
			}
		}
		
		/* check that the other end is still there... */
		if ( (second_now() - last_active) > AGENT_TIMEOUT ) {
			printf("connection seems broken - reconnecting...\n");
			break;
		}
	}

	printf("Exiting loop...\n");
	/* close all sockets (tunnel + managers) */
	for (i = 0; i < MAX_SNMP_MANAGERS + 1; i++){
		close(fds[i].fd);
	}
}

static void
parse_options(int argc, char *argv[])
{
	int c;

	while( (c = getopt(argc, argv, "c:sp:i:")) != -1) {
		switch(c)
			{
			case 'c':
				printf("Starting client...\n");
				is_client = true;
				snmp_manager_host = optarg;
				break;
			case 's':
				printf("Starting server...\n");
				is_server = true;
				break;
			case 'i':
				keep_alive_interval = atoi(optarg);
				printf("Keep-Alive interval is %d\n",keep_alive_interval);
				break;
			case 'p':
				snmp_manager_port = atoi(optarg);
				printf("Snmp Serve Port is %d\n",snmp_manager_port);
				break;
			case '?':
				printf("unknown option %c\n",c);
				break;
			default:
				printf("unknown option\n");
				break;
			}
	}
}


int
main(int argc, char *argv[])
{
	int listenfd;
	int bytes_read;
	int retval;
	int snmp_tunnel_sock, snmp_local_sock, snmp_trap_sock;

	parse_options(argc, argv);

	if (is_client) {
		snmp_manager_addr.sin_family = AF_INET;
		snmp_manager_addr.sin_port = htons(snmp_manager_port);
		retval = lookup_ip(snmp_manager_host,&(snmp_manager_addr.sin_addr));

		if (retval) {
			printf("Can't get snmp_manager->sin_addr.s_addr\n");
			exit(1);
		}


		while(1) {
			snmp_tunnel_sock = snmp_tunnel_ap_connect(&snmp_manager_addr);
			snmp_trap_sock = snmp_trap_setup();
			snmp_tunnel_ap_run(snmp_tunnel_sock,snmp_trap_sock);
		}

	}
}

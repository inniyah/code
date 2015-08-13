/*
 * listener.c -- joins a multicast group and echoes all data it receives from the group to its stdout...
 *
 * Antony Courtney,	25/11/94
 * Modified by: Frdric Bastien (25/03/04)
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

int main(int argc, char *argv[]) {
	struct sockaddr_in addr;
	int fd;
	ssize_t nbytes;
	socklen_t addrlen;
	struct ip_mreq mreq;
	char msgbuf[MSGBUFSIZE];

	u_int yes=1;

	/* create what looks like an ordinary UDP socket */
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* allow multiple sockets to use the same PORT number */
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("Reusing ADDR failed");
		exit(EXIT_FAILURE);
	}

	/* set up destination address */
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
	addr.sin_port=htons(HELLO_PORT);

	/* bind to receive address */
	if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/* use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	/* now just enter a read-print loop */
	while (1) {
		addrlen=sizeof(addr);
		if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0, (struct sockaddr *) &addr,&addrlen)) < 0) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		puts(msgbuf);
	}

	return EXIT_SUCCESS;
}

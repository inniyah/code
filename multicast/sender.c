
/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"

int main(int argc, char *argv[]) {
	struct sockaddr_in addr;
	int fd;
	char * message="Hello, World!";

/*
     TTL    Scope
    ----------------------------------------------------------------------
       0    Restricted to the same host. Won't be output by any interface.
       1    Restricted to the same subnet. Won't be forwarded by a router.
     <32    Restricted to the same site, organization or department.
     <64    Restricted to the same region.
    <128    Restricted to the same continent.
    <255    Unrestricted in scope. Global.
*/

	int ttl = 1; /* max = 255 */

	/* create what looks like an ordinary UDP socket */
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(fd,IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
		perror("Setting TTL failed");
		exit(EXIT_FAILURE);
	}

	/* set up destination address */
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);
	addr.sin_port=htons(HELLO_PORT);

	/* now just sendto() our destination! */
	while (1) {
		if (sendto(fd,message,sizeof(message),0,(struct sockaddr *) &addr, sizeof(addr)) < 0) {
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}

	return EXIT_SUCCESS;
}

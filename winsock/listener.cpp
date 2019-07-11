/*
 * listener.cpp -- joins a multicast group and echoes all data it receives from
 *		the group to its stdout...
 *
 * Antony Courtney,	25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 * modified for MinGW (12/06/12)
 */

#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>


#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.255"
#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd, nbytes,addrlen;
    struct ip_mreq_source mreq;
    char msgbuf[MSGBUFSIZE];
    WSADATA wsaData;

    /* Initialise Winsock */
    if(WSAStartup (0x0202, &wsaData))
    {
  	    perror("startup");
  	    exit(1);
  	}

    char yes=1;            /*** MODIFICATION TO ORIGINAL */

    /* create what looks like an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
	    perror("socket");
    	exit(1);
    }


/**** MODIFICATION TO ORIGINAL */
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
        perror("Reusing ADDR failed");
        exit(1);
    }
/*** END OF MODIFICATION TO ORIGINAL */

     /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port=htons(HELLO_PORT);
     
    /* bind to receive address */
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
	  perror("bind");
	  exit(1);
    }
     
     /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(fd,IPPROTO_IP,IP_ADD_SOURCE_MEMBERSHIP,(char *)&mreq,sizeof(mreq)) < 0) {
	  perror("setsockopt");
	  exit(1);
    }

    /* now just enter a read-print loop */
    while (1) {
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
			       (struct sockaddr *) &addr,&addrlen)) < 0) {
	       perror("recvfrom");
	       exit(1);
	  }
	  puts(msgbuf);
    }
    return 0;
}


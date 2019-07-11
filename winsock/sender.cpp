/*
 * sender.cpp -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 * modified for MinGW 12/06/12
 */

#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>


#define HELLO_PORT 12345
#define HELLO_GROUP "172.16.31.255"

main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd, cnt;
    struct ip_mreq_source mreq;
    char message[16]="Hello, World! ";
    WSADATA wsaData;

    /* Initialise Winsock */
    if(WSAStartup (0x0202, &wsaData))
    {
  	    perror("startup");
  	    exit(1);
  	}

    /* create what looks like an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
	    perror("socket");
	    exit(1);
    }

    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);
    addr.sin_port=htons(HELLO_PORT);
     
    /* now just sendto() our destination! */
    while (1) {
	    if (sendto(fd,message,sizeof(message),0,(struct sockaddr *) &addr,
		     sizeof(addr)) < 0) {
	         perror("sendto");
	         exit(1);
	    }
        Sleep(10);
    }
}

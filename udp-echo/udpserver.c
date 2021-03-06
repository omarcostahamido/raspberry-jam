/* 
 * Server side implementation of UDP client-server model
 *
 * This was developed in order to understand round-trip UDP latency.
 * This work is int the public domain.
 *
 * This is a derivative work combining concepts from these two pages:
 * - https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 * - https://gist.github.com/suyash/0f100b1518334fcf650bbefd54556df9
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	4464 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int n; 
   socklen_t len;

	len = sizeof(cliaddr); //len is value/resuslt 

   while(1) {
      n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
            &len); 
      buffer[n] = '\0'; 
      printf("Client : %s\n", buffer); 
#ifdef MACH_LINUX
      sendto(sockfd, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
#endif
#ifdef MACH_OSX
      sendto(sockfd, (const char *)hello, strlen(hello), 
            0, (const struct sockaddr *) &cliaddr, 
            len); 
#endif
      printf("Hello message sent.\n"); 
   }
	
	return 0; 
} 


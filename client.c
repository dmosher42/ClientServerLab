/*
 ** client.c -- a stream socket client demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3514" //Nathan's port
//#define PORT "3509" //Dee's port
#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
    
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes, run;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	run = 1;
	char newcommand[100];
	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}
	
    
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    
	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
    

	while (run == 1)
    {
        // loop through all the results and connect to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                                 p->ai_protocol)) == -1) {
                perror("client: socket");
                continue;
            }
            
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("client: connect");
                continue;
            }
            
            break;
        }
        
        if (p == NULL) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;
        }
        
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                  s, sizeof s);
        printf("client: connecting to %s\n", s);
        
        freeaddrinfo(servinfo); // all done with this structure
        
        
        
        
        
        //Socket it currently open:
        //Queue the client for a command
        printf("Enter a command: ");
        scanf("%s", newcommand);
        while(getchar() != '\n') {}
        printf("%s\n", newcommand);
        
        if (strcmp(newcommand,"quit\n")==0)
        {
            int numbytesSend;
            if ((numbytesSend = send(sockfd, newcommand, strlen(newcommand), 0) == -1))
            {
                perror("recv");
                exit(1);
            }
            
            //Here we recieve a message
            if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                perror("recv");
                exit(1);
            }
            
            buf[numbytes] = '\0';
            printf("client: received '%s'\n",buf);
            close(sockfd);
        }
        else
        {
            run=0;
        }
        
    }
	printf("goodbye");
    
	
	return 0;
}


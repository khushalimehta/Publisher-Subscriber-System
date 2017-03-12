//
//  aosPublisher.c
//  
//
//

//#include "aosPublisher.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <pthread.h>
#include <string.h>
void *function();

int myportno;
int main(int argc, char *argv[]) {
    int sockfd, portno, n, i = 0;
    int k =0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int totalserv;
    char serv[256];
    char * totalserv1;
    char buffer[256], numServ[256],tempbuff1[256];
    pthread_t thread1;
    
    

    
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    portno = atoi(argv[2]);
    //portno =;
    
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        printf("ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    /* Now ask for a message from the user, this message
     * will be read by server
     */
        printf("Please enter port number of publisher: ");
        bzero((char *) &buffer, sizeof(buffer));
        strcat(buffer,"pub");
        n = write(sockfd, buffer, strlen(buffer));
        
        bzero((char *) &buffer, sizeof(buffer));
        bzero((char *) &tempbuff1, sizeof(tempbuff1));
        fgets(buffer,sizeof(buffer),stdin);
        myportno = atoi(buffer);
        sprintf(tempbuff1,"%d",portno);
        strcat(buffer,"#");
        strcat(buffer,tempbuff1);

        n = write(sockfd, buffer, strlen(buffer));
       // bzero(buffer,sizeof(buffer));

        
        
       


      
    
    
    
        
        printf("Enter the total number of services\n");
        scanf("%d",&totalserv);

        memset(buffer,'\0', sizeof(buffer));

        sprintf(buffer,"%d",totalserv);

        strcat(buffer,"|");
         while( k < totalserv)
         {
            printf("Please enter the services that you want to provide: %d ",k);
            scanf("%s",serv);
            strcat(buffer,serv);
            strcat(buffer,",");
            k++;


        }
                /* Send message to the server */
        n = write(sockfd, buffer, strlen(buffer));
        
        
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        
       
          pthread_create (&thread1, NULL, &function, NULL); 
    
    if(pthread_join (thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    
    return 0;
    close(sockfd);
    

}


void *function() {
int sockfd, newsockfd, portno, clilen, i = 0;
char buffer[256], numServ, tempBuff[256];
struct sockaddr_in serv_addr, cli_addr;
int  n;
char * token;


/* First call to socket() function */
sockfd = socket(AF_INET, SOCK_STREAM, 0);

//struct info info1;

if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
}

/* Initialize socket structure */
bzero((char *) &serv_addr, sizeof(serv_addr));
portno = myportno;

serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(portno);

/* Now bind the host address using bind() call.*/
if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
}

/* Now start listening for the clients, here process will
 * go in sleep mode and will wait for the incoming connection
 */
printf("Waiting......\n");
while(1)
{
listen(sockfd,5);
clilen = sizeof(cli_addr);

/* Accept actual connection from the client */
        bzero((char *) &cli_addr, sizeof(cli_addr));

newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

if (newsockfd < 0) {
    perror("ERROR on accept");
    exit(1);
}

memset(buffer,'\0', sizeof(buffer));
n = read( newsockfd,buffer,sizeof(buffer) );
/* Write a response to the client */
n = write(newsockfd,"The service has been provided",50);

if (n < 0) {
    perror("ERROR writing to socket");
    exit(1);
}
return 0;
close(sockfd);
}
}




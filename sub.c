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


char *services[25] = {"AddTwoNumbers","SubtractTwoNumbers","MultiplyTwoNumbers","DivideTwoNumbers",
    "Square","SquareRoot","Area","Volume","Perimeter","Circumference","SurfaceArea","Integrate",
    "Differentiate","Power","Logarithm","StringLength","Encryption","Decryption","EdgeDetection",
    "FFT","RayTracing","VolumeRendering","ZBuffer","TextureMapping","MotionBlur"};

char buffer[256],tempbuffer[256];
char * token;

void *publisherfunction();
int portno;
int main(int argc, char *argv[])
 {
        pthread_t thread1;
        int sockfd;
        int newsockfd;
        struct sockaddr_in serv_addr; 
        struct hostent *server;
        int i,j,n;
        int ch; 
        int portno1;

        int flag = 0;
        char tempport[256];
        char port_tosend[5];
        printf("Available Services are as follow:\n");


        for(i=0,j=1;i<25;i++,j++)
        {
                printf("%d. %s\n", j,services[i]);
        }

        
        if (argc < 3)
        {
            printf("Require more arguments\n");
            exit(0);
        }
        portno = atoi(argv[2]);

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
            printf("\nError in opening socket\n");
            exit(1);
        }
        server = gethostbyname(argv[1]);
        if (server == NULL)
        {
            printf("No such host exist\n");
            exit(1);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);
  
        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
         {
            printf("ERROR connecting");
            exit(1);
        }
        bzero((char *) &buffer, sizeof(buffer));
        strcat(buffer,"sub|");
       
        sprintf(tempport,"%d",portno);
        strcat(buffer,tempport);
        n = write(sockfd, buffer, sizeof(buffer));
    
            printf("Enter the number of Service from the list:\n");
            bzero((char *) &buffer, sizeof(buffer));
            fflush(stdin);

            scanf("%s",buffer);

           
            if( write(sockfd , buffer , strlen(buffer)) < 0)
            {
                printf("Send Failed\n");
                return 1;
            }

            bzero(buffer,256);

            if( recv(sockfd , buffer , 256 , 0) < 0)
            {
                printf("Read Failed\n");
                flag = 1;
            }  
            printf("Path Found:%s\n",buffer);

            token = strtok(buffer,"->|");
            portno1 = atoi(token);


            
            
        pthread_create( &thread1, NULL, &publisherfunction, (void *)&portno1);

         if(pthread_join (thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
        }
        
        close(sockfd);
        return 0;
    
}

void *publisherfunction(void *ptr)
{
     int *pptr = (int *) ptr;
    int v = *pptr;
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(v);
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        exit(0);
    }
     
     
    //keep communicating with publisher
   
        printf("Enter service from the list provided by connected publisher : ");
        scanf("%s" , message);
         
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            exit(0);
        }
         
        //Receive a reply from the publisher
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
        }
         
        puts(server_reply);
    
     
    close(sock);
    return 0;
}
    	

 

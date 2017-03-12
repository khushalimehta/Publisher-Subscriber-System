//
//  broker.c
//  
//
//  Created by Nakul Desai on 11/21/16.
//
//

//#include "broker.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#define max 1256
#include <fcntl.h> // for open
#include <unistd.h> // for close

struct networkconn{
    int source;
    int finalarr;
    int destination;
};
int temp_index = 0;
int flagfound = 0;
char temp1Buff[max];


struct info {
    int pubPort;
    int broPort;
    int serviceNum[25];

};
char pathBuff[max];
int temparry[100];
void print_path(int s,int d,int lines);
void print_path_getlines(int temp1,int brokerport);

struct info info1[25];
struct networkconn n1[50];

int bcount;
int acount;
int temp;
int totalservicenumber;
int globalines;

char match;
int matchserv;
char matcharr;
int services[25];
int pubbroport;


char portno[max];
struct networkconn networkconnArr[25];


#define NTHREAD 5
void readfile(FILE * fp);
void getuniqueport(int bcount);
void *brokerfunction(void *ptr);

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;
    FILE * fp;
    int count = 0;
    pthread_t cur_thread_id;
    pthread_t pthread1[NTHREAD];
    fp = fopen("networkinfo.txt","r");
    int i;
    readfile(fp);
    
    int tem_c = acount;
    int no = 0;
    for(no = 0; no < acount ; no++) {
        
        temp = networkconnArr[no].finalarr;
        pthread_create(&cur_thread_id,NULL,&brokerfunction,(void *)&temp);
        pthread1[no] = cur_thread_id;
        sleep(1);
        
    }

    for(i=0;i<acount;i++)
    {
        pthread_join(pthread1[i],NULL);
    }
    return 0;
}

void readfile(FILE * fp)
{
    char lines[max];
    char * token;
    bcount = 0;
    int combinecount = 0;
    while(fgets(lines,sizeof(lines),fp) != NULL)
    {
        int len=strlen(lines); //where buff is your char array fgets is using
        if(lines[len-1]=='\n')
            lines[len-1]='\0';
        printf("the lines are %s\n",lines);
        
        token = strtok(lines, "#");
        
        /* walk through other tokens */
       
        while( token != NULL )
        {
            
            networkconnArr[bcount].source = atoi(token);
            token = strtok(NULL, "#");
            bcount++;
        }
        
    }
    getuniqueport(bcount);
    globalines = bcount;
    
}

void getuniqueport(int bcount)
{
    int j;
    acount = 0;
    int i;
    int i1 = 0;
    for(i = 0; i < bcount; i++) {
        for ( j=0; j<i; j++){
            if (networkconnArr[i].source == networkconnArr[j].source)
                break;
        }
        
        if (i == j){
            /* No duplicate element found between index 0 to i */
            networkconnArr[i1].finalarr = networkconnArr[i].source;
            printf("no duplicate:%d\n",networkconnArr[i1].finalarr);
            acount++;
            i1++;
            
        }
    }
  

    
}

void *brokerfunction(void *ptr)
{
    int *pptr = (int *) ptr;
    int v = *pptr;

    int sockfd, newsockfd, portno, i = 0;
    char buffer[256], numServ, tempBuff[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    char * servicno;
    char * servicno1;
    int temp1;

    char *token, * token1;
    socklen_t clilen;
    int count = 0;
    
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = v;
    printf("Port number is%d\n",portno);
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
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    /* Accept actual connection from the client */
    while(1)
    {
        bzero((char *) &cli_addr, sizeof(cli_addr));
        printf(" +++++++++++++++ ========> Before Accept \n\n");
                   
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        //printf("After Accept for Port number : %d\n",portno);
        

        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }
        //memset(buffer,'\0', sizeof(buffer));
        
        bzero((char *) &buffer, sizeof(buffer));

        n = read( newsockfd,buffer,sizeof(buffer) );
        
        token = strtok(buffer,"|");
        
        
        if(strcmp(token,"sub") == 0)
        {

            servicno = strtok(NULL,"|");
             int brokerport;
             brokerport = atoi(servicno);

            bzero((char *) &buffer, sizeof(buffer));
            n = read( newsockfd,buffer,sizeof(buffer) );
            
    //         printf("Service from sub%s\n",buffer);
  //           printf("network lines %d\n",acount);
//             printf("total Service lines %d\n",totalservicenumber);

             matchserv = atoi(buffer);
             

            int x, y = 0;
            for(x=0;x<acount;x++)
            {
                fflush(stdin);
            
                for(y=0;y<totalservicenumber;y++)
                {
                    if((info1[x].serviceNum[y]) == matchserv && info1[x].serviceNum[y] != 0 ){
                        printf("Service obtain at::%d\n",x);
                        temp1 = info1[x].pubPort;
                        sprintf(&match,"%d",temp1);
                        strcat(&match,"#");
                        break;
                        
                        

                    }

                }
            }
        print_path_getlines(pubbroport,brokerport);

        bzero((char *) &temp1Buff, sizeof(temp1Buff));
        sprintf(temp1Buff,"%d",temp1);
        strcat(temp1Buff,"->|");
        strcat(temp1Buff,pathBuff);
            n = write(newsockfd,temp1Buff,strlen(temp1Buff));

        }
        else
        {
            

            
            memset(buffer,'\0', sizeof(buffer));
            n = read( newsockfd,buffer,sizeof(buffer) );
                 if (n < 0) {
                perror("ERROR reading from socket");
                exit(1);
            }
            token1 = strtok(buffer,"#");
            bzero((char *) &tempBuff, sizeof(tempBuff));
            strcpy(tempBuff,token1);
            info1[count].pubPort = atoi(tempBuff);
            
            token1 = strtok(NULL,"#");
            bzero((char *) &tempBuff, sizeof(tempBuff));
            strcpy(tempBuff,token1);
            info1[count].broPort = atoi(tempBuff);
            pubbroport = atoi(tempBuff);
            
            memset(buffer,'\0', sizeof(buffer));
            n = read( newsockfd,buffer,sizeof(buffer) );
            
            servicno1 = strtok(buffer,"|");
            totalservicenumber = atoi(servicno1);
            servicno = strtok(NULL,",");

            /* get the first token */
            int k = 0;
             /* walk through other tokens */
            while( servicno != NULL ) 
            {
                
                while(info1[count].serviceNum[k]  != 0 && k < 25)
                {
                    k++;
                }
                info1[count].serviceNum[k] = atoi(servicno);
                k++;
                servicno = strtok(NULL,",");
            }
            
            
            printf("Publisher port number = %d\n", info1[count].pubPort);
            printf("Broker's port number = %d\n", info1[count].broPort);
            printf("Service number = %d\n", info1[count].serviceNum[0]);
            count++;
            
            n = write(newsockfd,"I got your message",18);
            
            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }
            
        }
    }
    return NULL;
    close(newsockfd);
}

void print_path(int s,int d,int lines)
{

    int i,j;
    int isVisited = 0;


    for(i = 0;i<lines;i++)
    {
       if(n1[i].source == 0)
        {   
            break;
        }
        isVisited = 0;

        if(n1[i].source == s)
        {       
            for(j=0;j<temp_index;j++)
                {
                    if (temparry[j] == n1[i].destination)
                    {
                        printf("visited==%d\n", temparry[j]);
                        isVisited = 1;
                        break;
                    }
                }

            if (isVisited)
                continue;
            if(flagfound == 0)
            {
            printf("%d -- %d\n",n1[i].source,n1[i].destination );
           // bzero((char *) &pathBuff, sizeof(pathBuff));
            int sd;
            sd = n1[i].source;
            char sourceBuff[20];

            sprintf(sourceBuff,"%d",sd);
            strcat(pathBuff,sourceBuff);
            strcat(pathBuff,"->");
            
            sd = n1[i].destination;
            char destbuffer[20];
            
            sprintf(destbuffer,"%d",sd);
            strcat(pathBuff,destbuffer);
            strcat(pathBuff,"|");

            
            }
            
            if(n1[i].destination == d)
            {
                printf("\npath found\n");
                flagfound = 1;
                break;
            }
            else
            {
                temparry[temp_index] = n1[i].source;
                temp_index++;
                temparry[temp_index] = n1[i].destination;
                temp_index++;                
                print_path(n1[i].destination,d,lines);
            }
        }
    }
    if(flagfound == 1)
    {
        return;
    }
}

void print_path_getlines(int temp1,int brokerport)
{
    char line[100];
    
    char *token2,*token1;
    char tempchar[20];
    int i,k=0,j;
    int scount = 0;
    int s,d;

    FILE *fp = fopen("networkinfo.txt", "r");
    if (fp == NULL) 
    {
        printf("Error!");   
        exit(1);
    }   
    printf("1\n");
    while(fgets(line, sizeof line, fp) != NULL)
    {
        
        int len=strlen(line); //where buff is your char array fgets is using
        if(line[len-1]=='\n')
           line[len-1]='\0';
        token1 = strtok(line,"#");
        
        n1[scount].source = atoi(token1);
        
        token2 = strtok(NULL," ");
        
        n1[scount].destination = atoi(token2);
        
        scount++;
    }
    
    for(i = 0; i<scount;i++)
    {
        
        printf("network ::: %d-->%d\n", n1[i].source,n1[i].destination );
    }
   
    for(i=0;i<100;i++)
    {
        temparry[i] = 0;
    }
    bzero((char *) &pathBuff, sizeof(pathBuff));

    print_path(temp1,brokerport,scount);
    
}


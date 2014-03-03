#include "network.h"
#include <stdio.h>
#include <sys/socket.h>

int readLine(char* buff,int bufflen,int sockfd) {
    int i=0;
    size_t rbytes = recv(sockfd,buff,1,0);
    i++;
    while(rbytes==1 && i<bufflen-1) {
        if(*buff=='\n') { 
            buff++;
            *buff='\0';
            return i; 
        }
        buff++;
        rbytes = recv(sockfd,buff,1,0);
    }
    printf("Something went horribly wrong on a read...\n");
    *buff='\0';
    return i;
    
}

int writeLine(char* buff, int len, int sockfd) {
    size_t sbytes = send(sockfd, buff, len, 0);
    while(sbytes < len) {
        len-=sbytes;
        buff+=sbytes;
        sbytes = send(sockfd, buff, len, 0);
    }
    return len;
}
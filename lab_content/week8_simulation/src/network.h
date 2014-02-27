#ifndef _NETWORK_H
#define _NETWORK_H

int readLine(char* buff,int bufflen,int sockfd);
int writeLine(char* buff, int len, int sockfd);

#endif
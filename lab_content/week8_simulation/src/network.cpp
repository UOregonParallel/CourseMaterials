#include "network.h"

#define BUFLEN 1024

void network_send(FILE* s, Vehicle* v, std::vector<Vehicle*> others) {
    char buf[BUFLEN];
    Location l=v->getLocation();
    snprintf(buf,BUFLEN,"::ME @(%f,%f)\n",l.x,l.y);
    fputs(buf,s);
    fputs("::BeginMarkers\n",s);
    for(int i=0;i<others.size();i++) {
        Vehicle* c=others[i];
        l = c->getLocation();
        snprintf(buf,BUFLEN,"(%f,%f)\n",l.x,l.y);
        fputs(buf,s);
    }
    fputs("::EndMarkers\n",s);
}

void network_recv(FILE* s, float* a, float* h) {
    int err;
    char buf[BUFLEN];
    fputs("::Action <accel delta> <heading delta>\n",s);
    fgets(buf,BUFLEN,s);
    err = sscanf(buf,"%f %f\n",a,h);
    if(err<2) {
        printf("Bad input!\n");
        *a=0; *h=0;
    }
}

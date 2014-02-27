#include "ControlledVehicle.h"

#include <stdio.h>
#include <string.h>
#include "network.h"

#include <vector>
#include "Location.h"

static const int BUFFLEN=512;

ControlledVehicle::ControlledVehicle() {
    sockfd=0;
}

void ControlledVehicle::setControlStream(int s) {
    sockfd = s;
}

void ControlledVehicle::ai(std::vector<Vehicle*> others) {
    float acc, trn;
    float x, y;
    int err;
    char buff[BUFFLEN];
    
    //sendNearbyOthers(others);
    loc.getCoordinate(&x, &y);
    err=snprintf(buff,BUFFLEN,"::Me\n%f %f\n", x, y);
    err=writeLine(buff, strlen(buff)+1, sockfd);
    if(err<1) {
        printf("Client has gone bad!\n");
        return;
    }
    err=snprintf(buff,BUFFLEN,"::BeginMarkers\n");
    err=writeLine(buff, strlen(buff)+1, sockfd);
    if(err<1) {
        printf("Client has gone bad!\n");
        return;
    }
    for(int i=0; i<others.size(); i++) {
        Location ol=others[i]->getLocation();
        if(loc.distance(&ol) < 100) {
            ol.getCoordinate(&x, &y);
            err=snprintf(buff,BUFFLEN,"%f %f\n", x, y);
            err=writeLine(buff, strlen(buff)+1, sockfd);
            if(err<1) {
                printf("Client has gone bad!\n");
                return;
            }
        }
    }
    err=snprintf(buff,BUFFLEN,"::EndMarkers\n");
    err=writeLine(buff, strlen(buff)+1, sockfd);
    if(err<1) {
        printf("Client has gone bad!\n");
        return;
    }
    
    //recieveUpdate(&acc,&trn);
    err=readLine(buff, BUFFLEN, sockfd);
    err=sscanf(buff,"%f %f\n",&acc,&trn);
    if(err<1) {
        printf("Client has gone bad!\n");
        return;
    }
    heading += trn;
    accel   += acc;
}
#include "ControlledVehicle.h"

#include "network.h"

ControlledVehicle::ControlledVehicle(): Vehicle() {
    net = NULL;
}

ControlledVehicle::ControlledVehicle(float x, float y): Vehicle(x,y) {
    net = NULL;
}

void ControlledVehicle::ai(std::vector<Vehicle*> others) {
    float a,h;
    network_send(net, this, others);
    network_recv(net, &a,&h);
    if(accel + a > maxaccel) { 
        accel = maxaccel;
    } else {
        accel+=a;
    }
    heading += h;
}

void ControlledVehicle::setControlStream(int sockfd) {
    net = fdopen(sockfd,"r+");
}

#ifndef _CONTROLLEDVEHICLE_H
#define _CONTROLLEDVEHICLE_H

#include "Vehicle.h"
#include <vector>

class ControlledVehicle: public Vehicle {
    int sockfd;
    
    public:
        ControlledVehicle();
        void setControlStream(int sockfd);
        void ai(std::vector<Vehicle*> others);
};

#endif
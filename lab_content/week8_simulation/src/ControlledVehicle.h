#ifndef _CONTROLLED_VEHICLE_H
#define _CONTROLLED_VEHICLE_H

#include "Vehicle.h"

class ControlledVehicle: private Vehicle {
    FILE* net;
    public:
        ControlledVehicle();
        ControlledVehicle(float x, float y);
        void setControlStream(int sockfd);
        void ai(std::vector<Vehicle*> others);
};

#endif

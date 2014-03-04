#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "Location.h"
#include <vector>

class Vehicle {
    private:
        float velocity;
        float maxvelocity;
        float maxaccel;
        float radius;
    
    protected:
        Location loc;
        float heading;
        float accel;
        
    public:
        Vehicle();
        Vehicle(float x, float y);
        void move(float timedelta);
        int collisionWith(Vehicle* other);
        void processCollision(Vehicle* other);
        Location getLocation();
        
        virtual void ai(std::vector<Vehicle*> others);
};

#endif

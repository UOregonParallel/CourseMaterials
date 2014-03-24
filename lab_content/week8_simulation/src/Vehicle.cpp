#include "Vehicle.h"
#include <stdio.h>

Vehicle::Vehicle() {
    heading=0.0;
    velocity=0.0;
    accel=0.0;
    maxvelocity=16.0;
    maxaccel=4.0;
    radius=1.0;
    loc=Location(0.0,0.0);
}

Vehicle::Vehicle(float x, float y) {
    heading=0.0;
    velocity=0.0;
    accel=0.0;
    maxvelocity=16.0;
    maxaccel=4.0;
    radius=1.0;
    loc=Location(x,y);
}

void Vehicle::move(float timedelta) {
    // Check that the vehicle is with motion spec
    if(accel>maxaccel) { accel=maxaccel; }
    velocity = velocity+accel*timedelta;
    if(velocity>maxvelocity) { velocity=maxvelocity; }
    
    loc.polarMove(heading, velocity);
}

int Vehicle::collisionWith(Vehicle* other) {
    float d=loc.distance(other->loc);
    float r=radius+other->radius;
    if(r>d) {
        return 1;
    }
    return 0;
}

void Vehicle::processCollision(Vehicle* other) {
    maxvelocity=0;
    maxaccel=0;
    velocity=0;
}

Location Vehicle::getLocation() {
    return loc;
}
        
void Vehicle::ai(std::vector<Vehicle*> others) {
    heading += M_PI/100;
}

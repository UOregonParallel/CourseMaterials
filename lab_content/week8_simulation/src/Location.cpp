#include "Location.h"
#include <math.h>

Location::Location() {
    x=0.0;
    y=0.0;
}

Location::Location(float x, float y) {
    this->x=x;
    this->y=y;
}

void Location::getCoordinate(float* x, float* y) {
    *x=this->x;
    *y=this->y;
}

void Location::getTileCoordinate(int* x, int* y) {
    *x=(int)(this->x-0.5);
    *y=(int)(this->y-0.5);
}

void Location::cartesianMove(float x, float y) {
    this->x+=x;
    this->y+=y;
}

void Location::polarMove(float radians, float dist) {
    float x=dist*cos(radians);
    float y=dist*sin(radians);
    cartesianMove(x,y);
}

float Location::distance(Location* other) {
    float dx=this->x-other->x;
    float dy=this->y-other->y;
    return sqrt(dx*dx+dy*dy);
}
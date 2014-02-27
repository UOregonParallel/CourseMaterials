#ifndef _LOCATION_H
#define _LOCATION_H

# define M_PI		3.14159265358979323846	/* pi */

class Location {
    float x;
    float y;
    
    public:
        Location();
        Location(float x, float y);
        
        void getCoordinate(float* x, float* y);
        void getTileCoordinate(int* x, int* y);
        void cartesianMove(float x, float y);
        void polarMove(float radians, float dist);
        float distance(Location* other);
};

#endif
#ifndef _LOCATION_H
#define _LOCATION_H

#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062


class Location {
    public:
        float x,y;

        Location();
        Location(float x, float y);
        void getTileCoordinate(int* x, int* y);
        void cartesianMove(float x, float y);
        void polarMove(float radians, float dist);
        float distance(Location other);
};

#endif _LOCATION_H

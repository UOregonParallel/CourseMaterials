#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Vehicle.h"

void network_send(FILE* s, Vehicle* v, std::vector<Vehicle*> others);
void network_recv(FILE* s, float* a, float* h);

#endif

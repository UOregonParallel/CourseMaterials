
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include "Vehicle.h"
#include "ControlledVehicle.h"

const float timedelta=.1;

std::vector<Vehicle*> vehicles;
std::vector<int> sockets;

void initBaseVehicles() {
    Vehicle* v=new Vehicle(10.0, 10.0);
    vehicles.push_back(v);
}

void updateLocations() {
    for(int i=0;i<vehicles.size();i++) {
        vehicles[i]->move(timedelta);
    }
}

void processCollisions() {
    for(int i=0;i<vehicles.size();i++) {
        Vehicle* a=vehicles[i];
        for(int j=i+1;j<vehicles.size();j++) {
            if(a->collisionWith(vehicles[i])) {
                a->processCollision(vehicles[i]);
                vehicles[i]->processCollision(a);
            }
        }
    }
}

void updateSettings() {
    for(int i=0;i<vehicles.size();i++) {
        vehicles[i]->ai(vehicles);
    }
}

void handleConnection(int sockfd) {
    ControlledVehicle* v=new ControlledVehicle();
    v->setControlStream(sockfd);
    vehicles.push_back(v);
}

void startNetwork(int port) {
    //// Begin Socket code... take CIS432 interested in what this all means
    struct sockaddr_in serv_addr; 
    struct sockaddr_in clnt_addr;
    socklen_t clnt_len;
    
    int srvsockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(srvsockfd < 0) {
        printf("Socket could not be acquired!\n");
        return;
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    ((char*)&serv_addr.sin_addr.s_addr)[0] = 127;
    ((char*)&serv_addr.sin_addr.s_addr)[1] = 0;
    ((char*)&serv_addr.sin_addr.s_addr)[2] = 0;
    ((char*)&serv_addr.sin_addr.s_addr)[3] = 1;
    serv_addr.sin_port = htons(port);
    if(bind(srvsockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Socket bind failed!\n");
        return;
    }
    if(listen(srvsockfd,1)<0) {
        printf("Impossible listen failure\n");
        return;
    }
    clnt_len = sizeof(clnt_addr);
    //// End Socket code... take CIS432 interested in what this all means
    
    //
    // !! need to repeat this part in a way that doesn't lock up the program
    //
    while(1) {
        int sockfd = accept(srvsockfd, (struct sockaddr*) &clnt_addr, &clnt_len);
        handleConnection(sockfd);
    }
}

int main(int argc, char** argv) {
    // setup basic vehicles
    initBaseVehicles();
    
    /// OpenMP may or may not immediately start execution of a task... the
    /// runtime is free to decide. Using the section construct is better.
    
    // accept connection
    #pragma omp parallel num_threads(2)
    #pragma omp sections
{
    #pragma omp section
    {
    printf("net started\n");
    startNetwork(8866);
    }

    #pragma omp section
    {
    printf("not net running\n");
    // run simulation
    while(1) {
        // handle the move phase
        updateSettings();
        updateLocations();
        processCollisions();
    }
    }
}
}

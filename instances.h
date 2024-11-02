// leerarchivo.h
#ifndef INSTANCES_H
#define INSTANCES_H

#include <vector>
#include <string>

struct Hotel {
    double x;
    double y;
    int score;
};

struct POI {
    double x;
    double y;
    int score;
};

void instances(const std::string& nombreArchivo, int& N, int& H, int& D, double& Tmax, std::vector<double>& Td, std::vector<Hotel>& hoteles, std::vector<POI>& pois);

#endif 

// leerarchivo.h
#ifndef INSTANCES_H
#define INSTANCES_H

#include <vector>
#include <string>

struct Vertex {
    std::string type; 
    int id;
    double x;
    double y;
    int score;
};

struct Solucion {
    std::vector<Vertex> tour;  
    double puntajeTotal;             
};

void instances(const std::string& nombreArchivo, int& N, int& H, int& D, double& Tmax, std::vector<double>& Td, std::vector<Vertex>& hoteles, std::vector<Vertex>& pois);

#endif 

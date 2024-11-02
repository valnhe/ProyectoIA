// main.cpp
#include <iostream>
#include "instances.h"
#include "initialsolution.h"

int main() {
    int N, H, D;
    double Tmax;
    std::vector<double> Td;
    std::vector<Hotel> hoteles;
    std::vector<POI> pois;

    std::string nombreArchivo = "instancias/64-45-1-2.ophs"; // Cambia esto por el nombre de tu archivo
    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois);

    SolucionInicial solucionInicial = generarSolucionInicial(hoteles, pois, Td, D);

    for (const auto& item : solucionInicial.ruta) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    std::cout << "Puntaje total: " << solucionInicial.puntajeTotal << std::endl;

    return 0;
}



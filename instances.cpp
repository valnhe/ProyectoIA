#include "instances.h"
#include <iostream>
#include <fstream>

void instances(const std::string& nombreArchivo, int& N, int& H, int& D, double& Tmax, std::vector<double>& Td, std::vector<Hotel>& hoteles, std::vector<POI>& pois) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    std::string linea;

    // Leer N, H, D
    archivo >> N >> H >> D;
    std::getline(archivo, linea);

    // Leer Tmax
    archivo >> Tmax;
    std::getline(archivo, linea);

    // Leer Td
    Td.resize(D);
    for (int i = 0; i < D; ++i) {
        archivo >> Td[i];
    }

    std::getline(archivo, linea);

    // Leer hoteles
    hoteles.resize(H + 2);
    for (int i = 0; i < H+2; ++i) {
        std::getline(archivo, linea);
        archivo >> hoteles[i].x >> hoteles[i].y >> hoteles[i].score;
    }

    pois.resize(N - 2);
    for (int i = 0; i < N - 2; ++i) {
        std::getline(archivo, linea);
        archivo >> pois[i].x >> pois[i].y >> pois[i].score;
    }
    
    archivo.close();
}

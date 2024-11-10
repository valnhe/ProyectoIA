#include "readInstance.h"
#include <iostream>
#include <fstream>

/**
 * Lee los datos de la instancia desde un archivo y los almacena en variables correspondientes.
 *
 * @param nombreArchivo El nombre del archivo que contiene los datos de instancia
 * @param N Variable donde se almacenará el número total de puntos de interés (POIs) + 2
 * @param H Variable donde se almacenará el número total de hoteles - 2
 * @param D Variable donde se almacenará el número total de trips del tour
 * @param Tmax Variable donde se almacenará el tiempo máximo permitido para el tour
 * @param Td Vector donde se almacenarán los tiempos máximos para cada trip
 * @param hoteles Vector donde se almacenarán los hoteles leídos del archivo
 * @param pois Vector donde se almacenarán los puntos de interés (POIs) leídos del archivo
 */

void instances(const std::string& nombreArchivo, 
                int& N, int& H, int& D, 
                double& Tmax, std::vector<double>& Td, 
                std::vector<Vertex>& hoteles, 
                std::vector<Vertex>& pois) {

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

    // Leer cada Td
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
        hoteles[i].id = i;
        hoteles[i].type = "H";

    }

    // Leer POIS
    pois.resize(N - 2);
    for (int i = 0; i < N - 2; ++i) {
        std::getline(archivo, linea);
        archivo >> pois[i].x >> pois[i].y >> pois[i].score;
        pois[i].id = i;
        pois[i].type = "P";

    }
    
    archivo.close();
}






#include <iostream>
#include "readInstance.h"
#include "randomSolution.h"
#include "funcionesAuxiliares.h"
#include "hc.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nombreArchivo>" << std::endl;
        return 1;
    }
    std::string nombreArchivo = argv[1];

    int N, H, D;
    double Tmax;
    std::vector<double> Td;
    std::vector<Vertex> hoteles;
    std::vector<Vertex> pois;

    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois);
    
    Solucion solucionInicial = generarSolucionInicial(hoteles, pois, Td, D);

    std::cout << " >>>> Solución Inicial:\n" << std::endl;
    std::cout << " Tour: ";
    for (const auto& item : solucionInicial.tour) {
        std::cout << item.type << item.id << " ";
        //cout <<"("<< item.x << "," << item.y << ") ";
    }
    std::cout << "\n" << " Puntaje total: " << solucionInicial.puntajeTotal << std::endl;
    std::cout << " Tiempo total utilizado: " << solucionInicial.tiempoTotal << std::endl;

    int restart = 10;
    int MAX_ITER = 100;

    Solucion mejorSolucion = hillClimbing (restart, MAX_ITER, solucionInicial, hoteles, pois, Td, D);

    std::cout << "\n\n - Iteraciones Terminadas - \n";
    std::cout << " >>>> Solución Inicial:\n" << std::endl;
    std::cout << " Tour: ";
    for (const auto& item : solucionInicial.tour) {
        std::cout << item.type << item.id << " ";
        //cout <<"("<< item.x << "," << item.y << ") ";
    }
    std::cout << "\n" << " Puntaje total: " << solucionInicial.puntajeTotal << std::endl;
    std::cout << " Tiempo total utilizado: " << solucionInicial.tiempoTotal << std::endl;

    std::cout << "\n >>>> Solución Local Encontrada:\n" << std::endl;
    std::cout << " Tour: ";
    for (const auto& item : mejorSolucion.tour) {
        std::cout << item.type << item.id << " ";

    }
    std::cout << "\n" <<  " Puntaje total Solución Final: " << mejorSolucion.puntajeTotal << std::endl;
    std::cout << " Tiempo total utilizado: " << mejorSolucion.tiempoTotal << std::endl;
    std::cout << "\n\n";
    std::cout << " ";

    return 0;
}



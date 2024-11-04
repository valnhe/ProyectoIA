#include <iostream>
#include "instances.h"
#include "initialsolution.h"
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

    int restart = 10;
    int MAX_ITER = 100;

    cout << " >>>> Solución Inicial:\n" << std::endl;
    cout << " Tour: ";
    for (const auto& item : solucionInicial.tour) {
        cout << item.type << item.id << " ";
    }
    cout << "\n" << " Puntaje total: " << solucionInicial.puntajeTotal << std::endl;
    double tiempo = calcularDistanciaTotal(solucionInicial.tour);
    cout << " Tiempo total utilizado: " << tiempo << std::endl;

    Solucion mejorSolucion = hillClimbing (restart, MAX_ITER, solucionInicial, hoteles, pois, Td, D);

    cout << "\n\n - Iteraciones Terminadas - \n";
    cout << " >>>> Solución Inicial:\n" << std::endl;
    cout << " Tour: ";
    for (const auto& item : solucionInicial.tour) {
        cout << item.type << item.id << " ";
        //cout <<"("<< item.x << "," << item.y << ") ";
    }
    cout << "\n" << " Puntaje total: " << solucionInicial.puntajeTotal << std::endl;
    cout << " Tiempo total utilizado: " << tiempo << std::endl;

    cout << "\n >>>> Solución Local Encontrada:\n" << std::endl;
    cout << " Tour: ";
    for (const auto& item : mejorSolucion.tour) {
        cout << item.type << item.id << " ";

    }
    cout << "\n" <<  " Puntaje total Solución Final: " << mejorSolucion.puntajeTotal << std::endl;
    double tiempofinal = calcularDistanciaTotal(mejorSolucion.tour);
    cout << " Tiempo total utilizado: " << tiempofinal << std::endl;
    cout << "\n\n";
    cout << " ";

    return 0;
}



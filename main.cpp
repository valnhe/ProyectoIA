#include <iostream>
#include "readInstance.h"
#include "randomSolution.h"
#include "funcionesAuxiliares.h"
#include "HillClimbing.h"
#include <fstream>
#include <chrono> 

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

    std::vector<Solucion> ejecuciones;
    int restart = 10;
    int MAX_ITER = 100;

    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois); //Ler achivo de instancia

    //Limpia el nombre para pasarselo al algoritmo. Sirve para saber si una instancia fue
    //incapaz de encontrar una secuencia factible de hoteles.
    std::string nombre_archivo;
     size_t pos = nombreArchivo.find_last_of("/");  //
    if (pos != std::string::npos) {
        nombre_archivo = nombreArchivo.substr(pos + 1);
    } else {
    }


    std::vector<Solucion> mejoresSoluciones = hillClimbing(restart, MAX_ITER, hoteles, pois, Td, D, nombre_archivo); // Ejecutar Hill Climbing

    Solucion mejorSol;
    mejorSol.puntajeTotal = 0.0;

    for (const auto& solucion : mejoresSoluciones) {
        if (solucion.puntajeTotal > mejorSol.puntajeTotal) {
            mejorSol = solucion;
        }
    }

    printearSoluciones(mejorSol);
    return 0;
}



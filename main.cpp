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

    int restart = 20;
    int MAX_ITER = 100;

    //Ler achivo de instancia
    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois);

    //Ejecutar Hill Climbing + MM
    std::vector<Solucion> mejoresSoluciones = hillClimbing (restart, MAX_ITER, hoteles, pois, Td, D);

    //Comparación solución inicial con solución local
    std::cout << "\n - Iteraciones Terminadas - \n";

    int i = 1;
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



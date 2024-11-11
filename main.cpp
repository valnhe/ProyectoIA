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

    int restart = 10;
    int MAX_ITER = 100;

    //Ler achivo de instancia
    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois);
    
    //Generar solución inicial
    Solucion solucionInicial = generarSolucionInicial(hoteles, pois, Td, D);
    std::cout << " >>>> Solución Inicial:\n" << std::endl;
    printearSoluciones(solucionInicial);

    //Ejecutar Hill Climbing + MM
    Solucion mejorSolucion = hillClimbing (restart, MAX_ITER, solucionInicial, hoteles, pois, Td, D);

    //Comparación solución inicial con solución local
    std::cout << "\n\n - Iteraciones Terminadas - \n";
    std::cout << " >>>> Solución Inicial:\n" << std::endl;
    printearSoluciones(solucionInicial);

    std::cout << "\n >>>> Solución Local Encontrada:\n" << std::endl;
    printearSoluciones(mejorSolucion);
    std::cout << "\n\n";
    std::cout << " ";

    return 0;
}



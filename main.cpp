#include <iostream>
#include "instances.h"
#include "initialsolution.h"

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
    std::vector<Hotel> hoteles;
    std::vector<POI> pois;

    instances(nombreArchivo, N, H, D, Tmax, Td, hoteles, pois);

    SolucionInicial solucionInicial = generarSolucionInicial(hoteles, pois, Td, D);

    cout << " " << std::endl;
    for (const auto& item : solucionInicial.ruta) {
        cout << item << " ";
    }
    cout << std::endl;
    cout << "Puntaje total: " << solucionInicial.puntajeTotal << "\n" << std::endl;

    return 0;
}



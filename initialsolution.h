#ifndef INICIALSOLUTION_H
#define INICIALSOLUTION_H

#include <vector>
#include <string>
#include "instances.h"

struct SolucionInicial {
    std::vector<std::string> ruta;  
    double puntajeTotal;             
};

SolucionInicial generarSolucionInicial(const std::vector<Hotel>& hoteles, const std::vector<POI>& poisDisponibles, const std::vector<double>& Td, int D);

#endif

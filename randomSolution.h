#ifndef INICIALSOLUTION_H
#define INICIALSOLUTION_H

#include <vector>
#include <string>
#include "readInstance.h"

Solucion generarSolucionInicial(const std::vector<Vertex>& hoteles, const std::vector<Vertex>& poisDisponibles, const std::vector<double>& Td, int D, const std::string& nombreArchivo);

#endif

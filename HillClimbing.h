#ifndef HILLCLIMBING_H_H
#define HILLCLIMBING_H_H

#include <vector>
#include <string>
#include "readInstance.h"

std::vector<Solucion> hillClimbing(int restart, int maxIter,
                      const std::vector<Vertex>& hoteles, const std::vector<Vertex>& pois, 
                      const std::vector<double>& Td, int D, const std::string& nombreArchivo);

#endif
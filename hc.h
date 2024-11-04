#ifndef HILLCLIMBING_H_H
#define HILLCLIMBING_H_H

#include <vector>
#include <string>
#include "instances.h"

Solucion hillClimbing(int restart, int maxIter, const Solucion& solucionInicial, 
                      const std::vector<Vertex>& hoteles, const std::vector<Vertex>& pois, 
                      const std::vector<double>& Td, int D);

#endif
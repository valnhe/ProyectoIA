#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "readInstance.h"

#include <cmath>

// Función para calcular la distancia entre un hotel y un POI
double calcularDistancia(const Vertex& h1, const Vertex& p);
std::vector<std::vector<Vertex>> dividirVector(const std::vector<Vertex>& tour);
double calcularDistanciaTotal(const std::vector<Vertex>& trip);
int calcularPuntajeTotal(const std::vector<Vertex>& t);


#endif // CALCULAR_DISTANCIA_H

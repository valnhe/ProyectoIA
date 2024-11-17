#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "readInstance.h"

#include <cmath>

double calcularDistancia(const Vertex& h1, const Vertex& p);
std::vector<std::vector<Vertex>> dividirVector(const std::vector<Vertex>& tour);
double calcularDistanciaTotal(const std::vector<Vertex>& trip);
int calcularPuntajeTotal(const std::vector<Vertex>& t);
void printearSoluciones (const Solucion& sol);
bool verificarChoques(Vertex nuevoVertice, const std::vector<Vertex>& tourActual);

#endif // CALCULAR_DISTANCIA_H

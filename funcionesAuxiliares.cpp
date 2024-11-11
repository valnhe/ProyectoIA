#include "funcionesAuxiliares.h"
#include <iostream>

/**
 * Calcula la distancia entre dos vértices
 *
 * @param Vertex Estructura que contiene las coordenadas de cada vértice
 */

double calcularDistancia(const Vertex& a, const Vertex& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}


/**
 * Divide un tour en los diferentes que lo componen trips
 *
 * @param tour Vector de vértices que contiene diferentes trips
 */
std::vector<std::vector<Vertex>> dividirVector(const std::vector<Vertex>& tour) {
    
    std::vector<std::vector<Vertex>> divisiones; // Para almacenar las divisiones
    std::vector<Vertex> divisionActual; // Para la división actual

    for (const auto& item : tour) {
        // Verificar si el elemento es un hotel
        if (item.type == "H") {
            // Si ya hay una división no vacía, quiere decir que es el último hotel del trip
            if (!divisionActual.empty()) {
                divisionActual.push_back(item); // Agregar el elemento actual a la división
                divisiones.push_back(divisionActual);
                divisionActual.clear(); // Limpiar para la próxima división
            }
        }
        divisionActual.push_back(item); // Agregar el elemento actual a la división
    }

    return divisiones;
}

/**
 * Calcula la distancia total en trip/tour
 *
 * @param t Vector de vértices a los cuáles se le calculará la distancia total
 */
double calcularDistanciaTotal(const std::vector<Vertex>& t) {
    double distanciaTotal = 0.0;

    for (size_t i = 0; i < t.size() - 1; ++i) {
        distanciaTotal += calcularDistancia(t[i], t[i + 1]);
    }

    return distanciaTotal;
}

/**
 * Calcula el puntaje total en trip/tour
 *
 * @param t Vector de vértices a los cuáles se le calculará el puntaje total
 */
int calcularPuntajeTotal(const std::vector<Vertex>& t) {
    int puntajeTotal = 0;

    for (const auto& vertex : t) {
        puntajeTotal += vertex.score;
    }

    return puntajeTotal;
}

/**
 * Printea una solución
 *
 * @param Solucion Solución que será printeada
 */
void printearSoluciones (const Solucion& sol) {
    std::cout << " Tour: ";
    for (const auto& item : sol.tour) {
        std::cout << item.type << item.id << " ";

    }
    std::cout << "\n" <<  " Puntaje total Solución Final: " << sol.puntajeTotal << std::endl;
    std::cout << " Tiempo total utilizado: " << sol.tiempoTotal << std::endl;
}
#include "funcionesAuxiliares.h"
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Calcula la distancia entre dos vértices
 *
 * @param Vertex Estructura que contiene las coordenadas de cada vértice
 */

double calcularDistancia(const Vertex& a, const Vertex& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/**
 * Divide un tour en los diferentes trips que lo componen 
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
    std::cout << "\n" <<  " Puntaje total recolectado: " << sol.puntajeTotal << std::endl;
    std::cout << " Tiempo total utilizado: " << sol.tiempoTotal << std::endl;
}


/**
 * Verifica si un vértice (hotel o punto de interés) tiene coordenadas que coincidan con algún vértice
 * en el tour actual. En caso de que exista un choque de coordenadas, retorna true, en caso contrario,
 * false.
 *
 * @param nuevoVertice Vértice que puede ser un hotel o un punto de interés que se va a comparar con los
 *                     vértices actuales del tour para buscar posibles choques de coordenadas.
 * @param tourActual Vector de vértices que representa el tour actual. Esta es la lista de hoteles y 
 *                   puntos de interés que se evalúa para verificar los choques de coordenadas.
 */
bool verificarChoques(Vertex nuevoVertice, const std::vector<Vertex>& tourActual) {
    for (auto& vertice : tourActual) {
        if (vertice.x == nuevoVertice.x && vertice.y == nuevoVertice.y) {
            // Hay choque
            return true; 
        }
    }
    // No hay choque
    return false;
}

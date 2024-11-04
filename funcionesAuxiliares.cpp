#include "funcionesAuxiliares.h"

double calcularDistancia(const Vertex& a, const Vertex& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

std::vector<std::vector<Vertex>> dividirVector(const std::vector<Vertex>& tour) {
    
    std::vector<std::vector<Vertex>> divisiones; // Para almacenar las divisiones
    std::vector<Vertex> divisionActual; // Para la división actual

    for (const auto& item : tour) {
        // Verificar si el elemento es un hotel
        if (item.type == "H") {
            // Si ya hay una división actual, agregarla a las divisiones
            if (!divisionActual.empty()) {
                divisionActual.push_back(item); // Agregar el elemento actual a la división
                divisiones.push_back(divisionActual);
                divisionActual.clear(); // Limpiar para la próxima división
            }
        }
        divisionActual.push_back(item); // Agregar el elemento actual a la división
    }

    //std::cout << "Divisiones del tour actual: " << divisiones.size() << std::endl;
    //for (const auto& item : divisiones) {
        //for (const auto& i : item) {
            //std::cout << i.type << i.id << " ";
        //}
        //std::cout << std::endl;
    //}

    return divisiones;
}

// Función para calcular la distancia total de un trip
double calcularDistanciaTotal(const std::vector<Vertex>& trip) {
    double distanciaTotal = 0.0;

    for (size_t i = 0; i < trip.size() - 1; ++i) {
        //std::cout << distanciaTotal <<"\n";
        distanciaTotal += calcularDistancia(trip[i], trip[i + 1]);
    }

    return distanciaTotal;
}

int calcularPuntajeTotal(const std::vector<Vertex>& t) {
    int puntajeTotal = 0;

    for (const auto& vertex : t) {
        puntajeTotal += vertex.score;
    }

    return puntajeTotal;
}
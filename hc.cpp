#include <vector>
#include <iostream>
#include <cmath>
#include <set>
#include <algorithm>
#include <limits>

#include "hc.h"
#include "funcionesAuxiliares.h"
#include "readInstance.h"


/**
 * Genero todos los vecinos al realizar el movimiento "Insertar Puntos de Interés"
 *
 * @param solucionActual Solución actual de una ejecución del Hill Climbing
 * @param todosHoteles Vector que contiene todos los Hoteles
 * @param pois Vector que contiene todos los puntos de Interés
 * @param Td Vector que contiene los tiempos máximos de cada trip
 *
 */
std::vector<Solucion> generarVecinosViaInsercion(const Solucion& solucionActual, const std::vector<Vertex>& todosPOIs, const std::vector<Vertex>& todosHoteles, const std::vector<double>& Td) {
    std::vector<Solucion> vecinos;
    std::set<int> poisEnTour;

    // Para revisar si un POI ya está incluido o no
    for (const auto& item : solucionActual.tour) {
        if (item.type == "P") {
            poisEnTour.insert(item.id);
        }
    }

    std::vector<std::vector<Vertex>> tripsSeparados = dividirVector(solucionActual.tour);

    // Iterar sobre cada POI existentes de la instancia
    for (const auto& poi : todosPOIs) {
        if (poisEnTour.find(poi.id) != poisEnTour.end()) {
            continue; // Saltar POIs que ya están en el tour
        }

        // Solución vacía
        Solucion bestPosition;
        bestPosition.puntajeTotal = 0.0;
        double mejorAumento = std::numeric_limits<double>::max();

        for (int i=0; i<tripsSeparados.size();i++) {

            std::vector<Vertex> trip = tripsSeparados[i];
            double initialDistance = calcularDistanciaTotal(trip);

            for (size_t j = 1; j < trip.size(); ++j) { // Insertar en todas las posiciones excepto la primera (hotel) y la última (hotel)
                std::vector<Vertex> newTrip = trip;
                newTrip.insert(newTrip.begin() + j, poi); // Insertar el POI en la posición j
                    
                double totalDistance = calcularDistanciaTotal(newTrip);
                double aumento = totalDistance - initialDistance;

                if (totalDistance <= Td[i] && aumento < mejorAumento) {
                    mejorAumento = aumento; // Actualizar el mejor aumento

                    std::vector<Vertex> nuevoTour;
                    for (int k = 0; k < tripsSeparados.size(); ++k) {
                        if (k == i) {
                            nuevoTour.insert(nuevoTour.end(), newTrip.begin(), newTrip.end() - 1); // Agregar el trip modificado sin el último hotel
                        } else {
                            nuevoTour.push_back(tripsSeparados[k][0]); // Agregar el primer hotel del trip
                            nuevoTour.insert(nuevoTour.end(), tripsSeparados[k].begin() + 1, tripsSeparados[k].end() - 1); // Agregar el contenido del trip sin el último hotel
                        }
                    }
                    nuevoTour.push_back(tripsSeparados.back().back()); // Agregar el último hotel del tour
                    bestPosition.tour = nuevoTour;
                    bestPosition.puntajeTotal = calcularPuntajeTotal(bestPosition.tour); 

                                   
                }
            }

        }

        if (bestPosition.tour.size() > solucionActual.tour.size()) { // Solo agregar si se agrega un POI
            vecinos.push_back(bestPosition);
        }
    }
    
    return vecinos;
}

/**
 * Ejecuta el Hill Climbing + MM
 *
 * @param restart Cantidad máxima de restarts del Hill Climbing (Sin implementar)
 * @param maxIter Cantidad máxima de iteraciones del Hill Climbing
 * @param solucionInicial Solución inicial de una ejecución del Hill Climbing
 * @param hoteles Vector que contiene todos los hoteles
 * @param pois Vector que contiene los puntos de interés (POIs)ttodos los POIs
 * @param Td Vector que contiene los tiempos máximos para cada trip
 * @param D Variable donde se almacenará el número total de trips del tour
 *
 */

Solucion hillClimbing(int restart, int maxIter, const Solucion& solucionInicial, 
                      const std::vector<Vertex>& hoteles, const std::vector<Vertex>& pois, 
                      const std::vector<double>& Td, int D) {
    
    //Se recibe la solInicial desde fuera, pero se debiera calcular aquí por los restart
    //Trabajo pendiente
    Solucion solucionActual = solucionInicial;
    int iteracion = 0;

    while (iteracion < maxIter) {

        //Se generan todos los vecinos
        std::vector<Solucion> vecinos = generarVecinosViaInsercion(solucionActual, pois, hoteles, Td);

        if (vecinos.empty()) {
            break; // No hay más vecinos
        }

        std::cout << "\n>> Iteración " << iteracion + 1;

        //Para ver todos los vecinos generados, se borrará luego.
        if (vecinos.size() > 0) { 
            std::cout << "\nVecinos generados:\n";
            for (const auto& vecino : vecinos) {
                std::cout << "Tour: ";
                for (const auto& vertex : vecino.tour) {
                    std::cout << vertex.type << vertex.id << " ";
                }
            std::cout << " | Puntaje Total: " << vecino.puntajeTotal << "\n";
            }
        }

        Solucion mejorVecino = solucionActual;
        for (const auto& vecino : vecinos) {
            if (vecino.puntajeTotal > mejorVecino.puntajeTotal) {
                mejorVecino = vecino;
            }
        }

        // Imprimir el mejor vecino encontrado en esta iteración
        std::cout << "\n" <<" >> Mejor vecino encontrado con puntaje total: " << mejorVecino.puntajeTotal << "\n >> Tour: ";
        for (const auto& vertex : mejorVecino.tour) {
            std::cout << vertex.type << vertex.id << " ";
        }
        std::cout << std::endl;

        //Solo si se encuentra un mejor vecino, se continua con la iteración
        if (mejorVecino.puntajeTotal > solucionActual.puntajeTotal) {
            solucionActual = mejorVecino;
            std::cout << " > El mejor vecino es mejor que la solución actual.\n";
        } else {
            std::cout << " > No se encontraron vecinos mejor que la solución actual.\n";
            break; // No hay mejora, salir del bucle
        }

        iteracion++;
    }

    //Se calcula el tiempoTotal de la soluciónActual y se retorna
    solucionActual.tiempoTotal = calcularDistanciaTotal(solucionActual.tour);
    return solucionActual; 
}




#include <vector>
#include <iostream>
#include <cmath>
#include <set>
#include <algorithm>
#include <limits>

#include "hc.h"
#include "funcionesAuxiliares.h"
#include "readInstance.h"

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

        //std::cout << "\n\n" << ">>>POI " << poi.id << "\n";

        ///Solucion vacia, puntaje total = 0
        Solucion bestPosition;
        bestPosition.puntajeTotal = 0.0;
        double mejorAumento = std::numeric_limits<double>::max();

        for (int i=0; i<tripsSeparados.size();i++) {

            //std::cout << "\n" <<">>Trip " << i + 1<< "\n";
            std::vector<Vertex> trip = tripsSeparados[i];
            double initialDistance = calcularDistanciaTotal(trip);

            for (size_t j = 1; j < trip.size(); ++j) { // Insertar en todas las posiciones excepto la primera (hotel) y la última (hotel)
                std::vector<Vertex> newTrip = trip;
                newTrip.insert(newTrip.begin() + j, poi); // Insertar el POI en la posición j
                    
                double totalDistance = calcularDistanciaTotal(newTrip);

                //std::cout << ">Iter " << j << "\n\n";
                //for (const auto& item : newTrip) {
                    //std::cout << item.type << item.id << " ";
                //}
    
                double aumento = totalDistance - initialDistance;
                
                //std::cout <<  "\n" << "Tiempo max" << Td[i] << "|";
                //std::cout << "Aumento" << aumento << "|";
                //std::cout << "Tiempo total" << totalDistance << "\n";

                if (totalDistance <= Td[i] && aumento < mejorAumento) {

                    //std::cout << "Aumento MEJOR" << aumento << "|";
                    //std::cout << "Tiempo total MEJOR" << totalDistance << "\n";
                    // Comprobar si el aumento es menor que el mejor aumento encontrado hasta ahora
                    
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

Solucion hillClimbing(int restart, int maxIter, const Solucion& solucionInicial, 
                      const std::vector<Vertex>& hoteles, const std::vector<Vertex>& pois, 
                      const std::vector<double>& Td, int D) {
    
    Solucion solucionActual = solucionInicial;
    int iteracion = 0;

    while (iteracion < maxIter) {
        std::vector<Solucion> vecinos = generarVecinosViaInsercion(solucionActual, pois, hoteles, Td);

        if (vecinos.empty()) {
            break; // No hay más vecinos que explorar
        }

        std::cout << "\n>> Iteración " << iteracion + 1;

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

        Solucion mejorVecino = vecinos[0];
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

        if (mejorVecino.puntajeTotal > solucionActual.puntajeTotal) {
            solucionActual = mejorVecino;
            std::cout << " > El mejor vecino es mejor que la solución actual.\n";
        } else {
            std::cout << " > No se encontraron vecinos mejor que la solución actual.\n";
            break; // No hay mejora, salir del bucle
        }

        iteracion++;
    }

    solucionActual.tiempoTotal = calcularDistanciaTotal(solucionActual.tour);

    return solucionActual; 
}




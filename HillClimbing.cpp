#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <set>
#include <algorithm>
#include <limits>

#include "HillClimbing.h"
#include "randomSolution.h"
#include "funcionesAuxiliares.h"
#include "readInstance.h"

/**
 * Realiza un Hill Climbing en un trip individual a través del movimiento swap. La función busca
 * mejorar el trip realizando intercambios de puntos de interés y comparando la distancia total
 * con el límite Td dado.
 *
 * @param trip Trip individual a ser optimizado
 * @param Td Tiempo máximo permitido para el trip
 */

std::vector<Vertex> swapTrip(const std::vector<Vertex>& trip, double Td) {
    
    int maxIter = 100;
    int iteracion = 0;
    std::vector<Vertex> tripActual = trip;

    while (iteracion < maxIter) {

        // >> Búsqueda de vecinos
        std::vector<std::vector<Vertex>> vecinos;
        std::vector<Vertex> vecino = tripActual;

        for (size_t j = 1; j < tripActual.size() - 2; ++j) { // Ignorar el primer y último elemento del trip ya que son hoteles
            std::swap(vecino[j], vecino[j + 1]); // Swaps entre puntos de interés continuos

            double newTripDistance = calcularDistanciaTotal(vecino);
            if (newTripDistance <= Td) { //Se revisa si es factible
                vecinos.push_back(vecino);
            }

            std::swap(vecino[j], vecino[j + 1]); // Revertir el swap
        }

        std::swap(vecino[1], vecino[trip.size() - 2]); // Swap entre el primer y el último punto de interés
        double newTripDistance = calcularDistanciaTotal(vecino);
        if (newTripDistance <= Td) { //Se revisa si es factible
            vecinos.push_back(vecino);
        }
        std::swap(vecino[1], vecino[trip.size() - 2]); // Revertir el swap
    
        // >> Búsqueda del mejor vecino
        std::vector<Vertex> mejorVecino = tripActual;
        double tiempoTotalMejorVecino = std::numeric_limits<double>::max();

        for (const auto& vecino : vecinos) { // Buscar el mejor vecino
            double tiempoVecino = calcularDistanciaTotal(vecino);
            if (tiempoVecino < tiempoTotalMejorVecino) {
                mejorVecino = vecino;
                tiempoTotalMejorVecino = tiempoVecino;
            }
        }

        if (tiempoTotalMejorVecino < calcularDistanciaTotal(tripActual)) { // Si encontramos un mejor vecino, iteramos nuevamente
            tripActual = mejorVecino;
        } else {
            break; // En caso contrario, no hay más mejoras y acaba el HC
        }
    }
    
    return tripActual;
}

/**
 * Se hace un HC individual en cada Trip a través del movimiento swap vía la función
 * "swapTrip". La función "swapTour" como tal junta toda la información recolectada 
 * y retorna una solución con el mejoramiento (si existe) del tour en su totalidad.
 *
 * @param solucionAct Solución actual de una ejecución del Hill Climbing recibida por swapTour
 * @param Td Vector que contiene los tiempos máximos de cada trip
 */

Solucion swapTour(Solucion solucionActual, const std::vector<double>& Td) {
    Solucion solucionSwap;
    std::vector<std::vector<Vertex>> tripsSeparados = dividirVector(solucionActual.tour);

    for (size_t i = 0; i < Td.size(); ++i) {
        std::vector<Vertex> trip = tripsSeparados[i];

        if (trip.size() > 2) { // Solo trabajar con el trip si tiene puntos de interés
            trip = swapTrip(trip, Td[i]);
        }

        // Agregar el trip (sin el último vértice para evitar duplicados)
        trip.pop_back();
        solucionSwap.tour.insert(solucionSwap.tour.end(), trip.begin(), trip.end());
    }

    // Agregar el último hotel y calcular todo lo necesario
    solucionSwap.tour.push_back(solucionActual.tour.back());
    solucionSwap.puntajeTotal = calcularPuntajeTotal(solucionSwap.tour);
    solucionSwap.tiempoTotal = calcularDistanciaTotal(solucionSwap.tour);

    return solucionSwap;
}

/**
 * Se mejoran los hoteles. Se selecciona un hotel al azar, y se buscan todos los
 * vecinos factibles para reemplazar.
 *
 * @param solucionAct Solución actual de una ejecución del Hill Climbing recibida por hotelImprovement
 * @param D Variable donde se almacenará el número total de trips del tour
 * @param todosHoteles Vector que contiene todos los Hoteles
 * @param Td Vector que contiene los tiempos máximos de cada trip
 *
 */

Solucion hotelImprovement (Solucion solucionAct, int D, const std::vector<Vertex>& hoteles, const std::vector<double>& Td) {
    Solucion solucionActual = solucionAct;
    int iteracion = 0;
    int maxIter = 100;

    while (iteracion < maxIter) {

        //El primer hotel del tour tiene índice 0, el segundo índice 1, así sucesivamente. 
        //Los que tengan índice 0 y D, son H0 y H1 respectivamente (inicial y final), por lo que
        //no se pueden cambiar y se ignoran en la distribución.

        std::random_device rd; 
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribucion(1, D - 1);
        int indiceHotelAMejorar = distribucion(gen);

        int indiceRealHotelAMejorar = 0;
        int contadorHoteles = 0;

        //Se busca en que posición está el hotel que se cambiará en el tour
        for (int i = 0; i < solucionActual.tour.size(); ++i) {
            if (solucionActual.tour[i].type == "H") {
                if (contadorHoteles == indiceHotelAMejorar) {
                    indiceRealHotelAMejorar = i;
                    break;
                }
                contadorHoteles++;
            }
        }

        // >> Búsqueda de vecinos
        //Para generar todos los vecinos
        std::vector<Solucion> vecinos; 

        for (int i = 0; i < hoteles.size() ; i++) { //Se cambiará el hotel seleccionado por cada uno de los hoteles disponibles.
            Solucion vecino = solucionActual;

            if (i == vecino.tour[indiceRealHotelAMejorar].id) { // Ignora el hecho de reemplazar un hotel por sí mismo.
                continue;
            }

            vecino.tour[indiceRealHotelAMejorar] = hoteles[i]; // Reemplazar el hotel seleccionado por el nuevo hotel

            //Se revisa que al cambiar el hotel no se exceda el máximo tiempo
            //disponible. Si excede alguno, ya no es una opción factible.
            std::vector<std::vector<Vertex>> tripsSeparados = dividirVector(vecino.tour);
            bool isValid = true;
            for (int i = 0; i<Td.size(); i++) {
                std::vector<Vertex> trip = tripsSeparados[i];
                double tripDistance = calcularDistanciaTotal(trip);

                if (tripDistance > Td[i] || verificarChoques(hoteles[i], solucionAct.tour)) {
                    isValid = false;
                }
            }

            // Si es verdadero, quiere decir que es un vecino factible.
            if (isValid) {
                vecino.puntajeTotal = calcularPuntajeTotal(vecino.tour); 
                vecino.tiempoTotal = calcularDistanciaTotal(vecino.tour); 
                vecinos.push_back(vecino);
            }
        }

        // >> Búsqueda del mejor vecino
        Solucion mejorVecino = solucionActual;
        mejorVecino.tiempoTotal = std::numeric_limits<double>::max();

        for (const auto& vecino : vecinos) {
            if (vecino.tiempoTotal < mejorVecino.tiempoTotal) {
                mejorVecino = vecino;
            }
        }
            
        if (mejorVecino.tiempoTotal < solucionActual.tiempoTotal) { //Solo si se encuentra un mejor vecino, se continua con la iteración
            solucionActual = mejorVecino;
        } else {
            break; // No hay mejora, salir del bucle
        }

        iteracion++;
    }
    return solucionActual;
}

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

    for (const auto& item : solucionActual.tour) { // Para revisar si un POI ya está incluido o no
        if (item.type == "P") {
            poisEnTour.insert(item.id);
        }
    }

    std::vector<std::vector<Vertex>> tripsSeparados = dividirVector(solucionActual.tour);

    // Iterar sobre cada POI existentes de la instancia
    for (const auto& poi : todosPOIs) {
        if (poisEnTour.find(poi.id) != poisEnTour.end() || verificarChoques(poi, solucionActual.tour)) {
            continue; // Saltar POIs que ya están en el tour o si hay choque de coordenadas
        }

        // >> Búsqueda de los vecinos
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
                    bestPosition.tiempoTotal = calcularDistanciaTotal(bestPosition.tour);
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
 * @param hoteles Vector que contiene todos los hoteles
 * @param pois Vector que contiene los puntos de interés (POIs)ttodos los POIs
 * @param Td Vector que contiene los tiempos máximos para cada trip
 * @param D Variable donde se almacenará el número total de trips del tour
 *
 */

std::vector<Solucion> hillClimbing(int maxRestart, int maxIter,
                      const std::vector<Vertex>& hoteles, const std::vector<Vertex>& pois, 
                      const std::vector<double>& Td, int D) {

    int restart = 0;
    std::vector<Solucion> solucionesGuardadas;

    while (restart < maxRestart) {

        Solucion solucionActual = generarSolucionInicial(hoteles, pois, Td, D);
        //printearSoluciones(solucionActual);
        int iteracion = 0;

        while (iteracion < maxIter) {

            //Se generan todos los vecinos            
            std::vector<Solucion> vecinos = generarVecinosViaInsercion(solucionActual, pois, hoteles, Td);

            if (vecinos.empty()) {
                break; // No hay más vecinos
            }

            Solucion mejorVecino = solucionActual;
            for (const auto& vecino : vecinos) {
                if (vecino.puntajeTotal > mejorVecino.puntajeTotal) {
                    mejorVecino = vecino;
                }
            }

            if (mejorVecino.puntajeTotal > solucionActual.puntajeTotal) { //Solo si se encuentra un mejor vecino, se continua con la iteración
                solucionActual = mejorVecino;
            } else {
                break; // No hay mejora, salir del bucle
            }    

            std::random_device rd; 
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distribucion(0, 2);
            double probabilidad = distribucion(gen);


            if (probabilidad < 2) {
                solucionActual = hotelImprovement(solucionActual, D, hoteles, Td);
                
            } else {
                solucionActual = swapTour(solucionActual, Td);
            }

            iteracion++;
    }
        solucionesGuardadas.push_back(solucionActual);
        restart++;
    }
    return solucionesGuardadas; 
}




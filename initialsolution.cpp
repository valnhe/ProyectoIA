#include <random>
#include <set>
#include <vector>
#include <iostream>
#include <cmath>
#include "instances.h"
#include "initialsolution.h"
#include "funcionesAuxiliares.h" 

/**
 * Selecciona hoteles de acuerdo a la cantidad de trips requeridos, siempre respetando el Td de cada trip
 * @param D Cantidad de trips del tour
 * @param hoteles Vector que almacena los hoteles disponibles para elegir
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 */

std::vector<Vertex> seleccionarHotelesAleatorios(int D, const std::vector<Vertex>& hoteles, const std::vector<double>& Td) {
    const int MAX_GLOBAL_ITER = 20; // Número máximo de iteraciones globales

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, hoteles.size() - 1); 

    for (int globalIter = 0; globalIter < MAX_GLOBAL_ITER; ++globalIter) {
        std::vector<Vertex> HotelesSeleccionados;
        HotelesSeleccionados.push_back(hoteles[0]); // Se incluye el hotel inicial H0

        bool validSelection = true;

        for (int i = 1; i < D; ++i) {
            bool hotelSinSeleccionar = true;
            int localIter = 0;

            while (hotelSinSeleccionar) {
                int indiceHotel = distribucion(gen);
                Vertex nuevoHotel = hoteles[indiceHotel];
                Vertex lastHotel = HotelesSeleccionados.back();

                double distancia = calcularDistancia(lastHotel, nuevoHotel);

                if (i == D-1) { // Verificar el penúltimo hotel
                    double distanciaFinal = calcularDistancia(nuevoHotel, hoteles[1]);

                    if (distancia < Td[i-1] && distanciaFinal < Td[i]) {
                        HotelesSeleccionados.push_back(nuevoHotel);
                        HotelesSeleccionados.push_back(hoteles[1]); 
                        hotelSinSeleccionar = false;
                    }
                } else {
                    if (distancia < Td[i-1]) {
                        HotelesSeleccionados.push_back(nuevoHotel);
                        hotelSinSeleccionar = false;
                    }
                }

                ++localIter;
                if (localIter > 1000) {
                    validSelection = false;
                    break;
                }
            }

            if (!validSelection) {
                break;
            }
        }

        if (validSelection) {
            return HotelesSeleccionados;
        }
    }

    std::cout << "No se pudo encontrar una solución válida después de " << MAX_GLOBAL_ITER << " intentos." << std::endl;
    return {};
}

/**
 * Selecciona puntos de interés entre cada par de hoteles, siempre respetando el Td de cada trip
 * @param h1, h2 Hotel de inicio y llegada del trip respectivamente
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 * @param pois Vector con todos los POIs
 * @param indicesSeleccionados Set que guarda los índices de los POIs ya utilizados
 */

std::vector<Vertex> creadorTrips(const Vertex& h1, const Vertex& h2, 
                                                double Td, 
                                                const std::vector<Vertex>& pois,
                                                std::vector<Vertex>& poisSeleccionados) {
    
    // Permite seleccionar POI aHl azar
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, pois.size() - 1);

    std::vector<Vertex> Trip;
    Trip.push_back(h1);

    int MAX_ITER = 0;
    while (MAX_ITER < 100) {

        MAX_ITER += 1;

        // Todos los POIs ya han sido seleccionados
        if (poisSeleccionados.size() == pois.size()) {
            std::cout << "Sin POIs disponibles\n";
            break; 
        }

        int indicePOI = distribucion(gen); // Se selecciona un POI al azar
        Vertex nuevoPOI = pois[indicePOI];

        // Verificar si el POI ya fue seleccionado
        for (const Vertex& poi : poisSeleccionados) {
            if (poi.id == nuevoPOI.id) {
                break; // Salir del bucle si se encuentra el POI
            }
        }

        Trip.push_back(nuevoPOI);
        Trip.push_back(h2);

        double distancia = calcularDistanciaTotal(Trip);

        if (distancia < Td) {
            Trip.pop_back(); // Quito el último hotel y continuo ingresando POIs si aún me queda tiempo
        } else {
            Trip.pop_back(); // Quito el último POI ingreso y el último Hotel ya que me pasé del tiempo.
            Trip.pop_back();
        }
    }

    return Trip;
}

/**
 * Se genera la solución inicial de manera aleatoria
 * @param hoteles Vector que almacena los hoteles disponibles para elegir
 * @param pois Vector con todos los POIs
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 * @param D Cantidad de trips del tour
 */

Solucion generarSolucionInicial(const std::vector<Vertex>& hoteles, 
                                        const std::vector<Vertex>& pois, 
                                        const std::vector<double>& Td, 
                                        int D) {
    
    std::vector<Vertex> hotelesSeleccionados = seleccionarHotelesAleatorios(D, hoteles, Td);   

    std::vector<Vertex> poisSeleccionados; // Acá se guardar los índices de los POIs ya utilizados

    Solucion resultado;
    resultado.puntajeTotal = 0.0;

    // Iterar sobre los pares de hoteles seleccionados, es decir, trabajará en cada Trip
    for (size_t i = 0; i < hotelesSeleccionados.size() - 1; ++i) {

        Vertex h1 = hotelesSeleccionados[i];
        Vertex h2 = hotelesSeleccionados[i + 1];
        double td = Td[i];

        auto Trips = creadorTrips(h1, h2, td, pois, poisSeleccionados);

        for (const auto& poi : Trips) {
            resultado.tour.push_back(poi); // Agrega el POI al tour
            resultado.puntajeTotal += poi.score; // Suma el puntaje del POI
        }
    }

    // Agregar el último hotel
    Vertex hf = hotelesSeleccionados.back();
    resultado.tour.push_back(hf);
    
    return resultado;
}



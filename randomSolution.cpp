#include <random>
#include <cmath>
#include <vector>
#include <iostream>
#include "readInstance.h"
#include "randomSolution.h"
#include "funcionesAuxiliares.h" 

/**
 * Selecciona hoteles de acuerdo a la cantidad de trips requeridos, siempre respetando el Td de cada trip
 * @param D Cantidad de trips del tour
 * @param hoteles Vector que almacena los hoteles disponibles para elegir
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 */

std::vector<Vertex> seleccionarHotelesAleatorios(int D, 
                                                    const std::vector<Vertex>& hoteles, 
                                                    const std::vector<double>& Td, const std::string& nombreArchivo) {
    const int MAX_GLOBAL_ITER = 50; // Número máximo de iteraciones globales

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, hoteles.size() - 1); 

    for (int globalIter = 0; globalIter < MAX_GLOBAL_ITER; ++globalIter) {
        std::vector<Vertex> HotelesSeleccionados;
        HotelesSeleccionados.push_back(hoteles[0]); // Se incluye el hotel inicial H0

        bool validSelection = true;
        for (int i = 1; i < D; ++i) {  // Para cada trip
            bool hotelSinSeleccionar = true;
            int localIter = 0;

            while (hotelSinSeleccionar) { // Se seleccionan hoteles al azar mientras quede pendiente

                int indiceHotel = distribucion(gen);
                Vertex nuevoHotel = hoteles[indiceHotel];
                Vertex lastHotel = HotelesSeleccionados.back();

                double distancia = calcularDistancia(lastHotel, nuevoHotel);

                if (i == D-1) { // Verificar el penúltimo hotel (que cumpla con la distancia entre el hotel anterior y el último hotel)
                    double distanciaFinal = calcularDistancia(nuevoHotel, hoteles[1]);

                    if (distancia < Td[i-1] && distanciaFinal < Td[i]) {
                        HotelesSeleccionados.push_back(nuevoHotel);
                        HotelesSeleccionados.push_back(hoteles[1]); // Se incluye el hotel final H1
                        hotelSinSeleccionar = false;
                    }

                } else { // Verificar cualquier otro hotel que no sea el penúltimo
                    if (distancia < Td[i-1]) {
                        HotelesSeleccionados.push_back(nuevoHotel);
                        hotelSinSeleccionar = false;
                    }
                }

                // Si tras 1000 iteraciones no se encuentra un hotel factible para
                // cierta posición, simplemente se abandona esa búsqueda.
                ++localIter;
                if (localIter > 1000) {
                    validSelection = false;
                    break;
                }
            }

            // Si validSelection es false, quiere decir que no se logró encontrar
            // una combinación factible, por lo que se comienza desde 0 e inicia otra
            // iteración global.
            if (!validSelection) {
                break;
            }
        }

        // Si efectivamente se encuentra una combinación de hoteles factible,
        // se retorna la combinación.
        if (validSelection) {
            return HotelesSeleccionados;
        }
    }

    // Si tras 20 iteraciones globales no se encuentra nada:
    std::cout << "No se pudo encontrar una combinación válida de hoteles después de " << MAX_GLOBAL_ITER << " intentos en la instancia" << nombreArchivo << std::endl;
    return {};
}

/**
 * Selecciona puntos de interés entre cada par de hoteles, siempre respetando el Td de cada trip
 * @param h1, h2 Hotel de inicio y llegada del trip respectivamente
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 * @param pois Vector con todos los POIs
 * @param indicesSeleccionados Set que guarda los índices de los POIs ya utilizados
 */

std::vector<Vertex> creadorTrips(const Vertex& h1, 
                                        const Vertex& h2, 
                                        double Td, 
                                        const std::vector<Vertex>& pois,
                                        std::vector<Vertex>& poisSeleccionados) {
    
    // Permite seleccionar POI al azar
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, pois.size() - 1);

    std::vector<Vertex> Trip;
    Trip.push_back(h1);

    int MAX_ITER = 0;
    while (MAX_ITER < 5000) {

        MAX_ITER += 1;

        // Todos los POIs ya han sido seleccionados
        if (poisSeleccionados.size() == pois.size()) {
            std::cout << "Sin POIs disponibles\n";
            break; 
        }

        int indicePOI = distribucion(gen); // Se selecciona un POI al azar
        Vertex nuevoPOI = pois[indicePOI];
        
        // Verificar si el POI ya fue seleccionado

        bool exists = false;
        for (const Vertex& poi : poisSeleccionados) {
            if (poi.id == nuevoPOI.id) {
                exists = true; // Salir del bucle si se encuentra el POI
            }
        }

        if (exists || verificarChoques(nuevoPOI, Trip)) {
            break;
        }

        //Ingreso el nuevo POI y el último hotel
        Trip.push_back(nuevoPOI);
        Trip.push_back(h2);

        double distancia = calcularDistanciaTotal(Trip);

        if (distancia < Td) { // Si la distancia es menor, probablemente me quede espacio para seguir ingresando
            Trip.pop_back(); // Quito el último hotel y continuo ingresando POIs si aún me queda tiempo
            poisSeleccionados.push_back(nuevoPOI);
        } else {
            Trip.pop_back(); // Quito el último POI ingresado y el último Hotel ya que se pasa del tiempo.
            Trip.pop_back();

            // Si ya tengo al menos un POI, tengo lo suficiente para ser una solucion inicial
            // por lo que abandono la búsqueda.
            if (Trip.size() > 2) { 
                break;
            }
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
                                        int D, const std::string& nombreArchivo) {
    
    std::vector<Vertex> hotelesSeleccionados = seleccionarHotelesAleatorios(D, hoteles, Td, nombreArchivo);   
    std::vector<Vertex> poisSeleccionados; // Acá se guardan los índices de los POIs ya utilizados

    Solucion resultado;
    resultado.puntajeTotal = 0.0;

    if (hotelesSeleccionados.size() > 2) {
        // Iterar sobre los pares de hoteles seleccionados, es decir, se trabaja en cada Trip
        for (size_t i = 0; i < hotelesSeleccionados.size() - 1; ++i) {

            Vertex h1 = hotelesSeleccionados[i];
            Vertex h2 = hotelesSeleccionados[i + 1];
            double td = Td[i];

            auto Trips = creadorTrips(h1, h2, td, pois, poisSeleccionados);

            for (const auto& ver : Trips) {
                resultado.tour.push_back(ver); // Agrega el Vértice al tour
                resultado.puntajeTotal += ver.score; // Suma el puntaje del Vértice
            }
        }

        // Agregar el último hotel
        Vertex hf = hotelesSeleccionados.back();
        resultado.tour.push_back(hf);
        resultado.tiempoTotal = calcularDistanciaTotal(resultado.tour);
        
        return resultado;
    }

    return resultado;
}



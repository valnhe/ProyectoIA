#include <random>
#include <set>
#include <vector>
#include <iostream>
#include <cmath>
#include "instances.h"
#include "initialsolution.h"


// Funciones que calculan la distancia euclidiana entre dos puntos


double calcularDistancia(const Hotel& h1, const Hotel& h2) {
    return std::sqrt((h1.x - h2.x) * (h1.x - h2.x) + (h1.y - h2.y) * (h1.y - h2.y));
}

double calcularDistancia(const POI& p1, const POI& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double calcularDistancia(const Hotel& h1, const POI& p1) {
    return std::sqrt((p1.x - h1.x) * (p1.x - h1.x) + (p1.y - h1.y) * (p1.y - h1.y));
}

/**
 * Selecciona hoteles de acuerdo a la cantidad de trips requeridos, siempre respetando el Td de cada trip
 * @param D Cantidad de trips del tour
 * @param hoteles Vector que almacena los hoteles disponibles para elegir
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 */

std::vector<int> seleccionarHotelesAleatorios(int D, 
                                                const std::vector<Hotel>& hoteles, 
                                                const std::vector<double>& Td) {
    
    std::vector<int> indicesSeleccionados; // Acá se guardan los índices de los hoteles seleccionados
    indicesSeleccionados.push_back(0); // Se incluye el hotel inicial H0

    // Permite seleccionar hoteles al azar
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, hoteles.size() - 1); 

    while (indicesSeleccionados.size() < D) {
        int nuevoHotel = distribucion(gen); // Se selecciona un hotel al azar
        bool cumpleCondicion = true;

        for (size_t i = 0; i < indicesSeleccionados.size(); ++i) {
            double distancia;
            if (i == 0) {
                distancia = calcularDistancia(hoteles[indicesSeleccionados[i]], hoteles[nuevoHotel]);
            } else {
                distancia = calcularDistancia(hoteles[indicesSeleccionados[i - 1]], hoteles[nuevoHotel]);
            }

            if (distancia >= Td[i]) {
                cumpleCondicion = false; // Si la distancia no cumple, no se puede seleccionar
                break;
            }
        }

        // Si la distancia es menor a Td, se ingresa como hotel seleccionado
        if (cumpleCondicion) {
            indicesSeleccionados.push_back(nuevoHotel);
        }
    }

    indicesSeleccionados.push_back(1); // Siempre incluir el hotel final H1
    return indicesSeleccionados;
}

/**
 * Selecciona puntos de interés entre cada par de hoteles, siempre respetando el Td de cada trip
 * @param h1, h2 Hotel de inicio y llegada del trip respectivamente
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 * @param pois Vector con todos los POIs
 * @param indicesSeleccionados Set que guarda los índices de los POIs ya utilizados
 */

std::pair<std::vector<POI>, std::vector<int>> seleccionarPOIsAleatorios(const Hotel& h1, const Hotel& h2, 
                                                                double Td, 
                                                                const std::vector<POI>& pois,
                                                                std::set<int>& indicesSeleccionados) {
    
    std::vector<POI> poisSeleccionados; // Acá se guarda la info de los POI seleccionados para el Trip
    std::vector<int> indicesSeleccionadosTrip; // Acá se guardan los índices de los POI seleccionados para el Trip

    // Permite seleccionar POI al azar
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, pois.size() - 1);

    double distanciaTotal = 0.0;
    int contadorPOIs = 0; // No permite trips vacíos

     while (true) {
        int indicePOI = distribucion(gen); // Se selecciona un POI al azar

        // Verificar si el POI ya fue seleccionado
        if (indicesSeleccionados.find(indicePOI) == indicesSeleccionados.end()) {

            POI nuevoPOI = pois[indicePOI];

            // Calcula distancia entre el último vértice y el POI. Si no hay POIs seleccionados,
            // quiere decir que el último vértice es el hotel de inicio.
            double distanciaDesdeUltimo = (poisSeleccionados.empty()) ? 
                                           calcularDistancia(h1, nuevoPOI) : 
                                           calcularDistancia(poisSeleccionados.back(), nuevoPOI);


            // Si la distancia desde el primer hotel hasta el POI seleccionado es menor a Td...
             if (distanciaTotal + distanciaDesdeUltimo <= Td) {

                //... Se revisa que deje tiempo disponible para llegar al hotel final.
                double distanciaAlFinal = calcularDistancia(h2, nuevoPOI);
                if (distanciaTotal + distanciaDesdeUltimo + distanciaAlFinal <= Td) {

                    //Al cumplirse las dos condiciones, se agrega el punto de interés el trip.
                    poisSeleccionados.push_back(nuevoPOI);
                    indicesSeleccionados.insert(indicePOI);
                    indicesSeleccionadosTrip.push_back(indicePOI);
                    distanciaTotal += distanciaDesdeUltimo;
                    contadorPOIs++;
                } else {
                    if (contadorPOIs > 0) {
                    break;
                }
                } 
                
            } else {
                if (contadorPOIs > 0) {
                    break;
                }
            }
        }
    }

    return {poisSeleccionados, indicesSeleccionadosTrip};
}

/**
 * Se genera la solución inicial de manera aleatoria
 * @param hoteles Vector que almacena los hoteles disponibles para elegir
 * @param pois Vector con todos los POIs
 * @param Td Vector donde se almacena los tiempos máximos para cada trip
 * @param D Cantidad de trips del tour
 */

SolucionInicial generarSolucionInicial(const std::vector<Hotel>& hoteles, 
                                        const std::vector<POI>& pois, 
                                        const std::vector<double>& Td, 
                                        int D) {
    std::vector<int> hotelesSeleccionados = seleccionarHotelesAleatorios(D, hoteles, Td);
    std::set<int> indicesSeleccionados; // Acá se guardar los índices de los POIs ya utilizados

    SolucionInicial resultado;
    resultado.puntajeTotal = 0.0;

    // Iterar sobre los pares de hoteles seleccionados, es decir, trabajará en cada Trip
    for (size_t i = 0; i < hotelesSeleccionados.size() - 1; ++i) {
        Hotel h1 = hoteles[hotelesSeleccionados[i]];
        Hotel h2 = hoteles[hotelesSeleccionados[i + 1]];
        double td = Td[i];

        // Seleccionar POIs entre h1 y h2
        auto [poisEntre, indicesPOIs] = seleccionarPOIsAleatorios(h1, h2, td, pois, indicesSeleccionados);

        // Agregar hotel y POIs al resultado final
        resultado.ruta.push_back("H" + std::to_string(hotelesSeleccionados[i]));
        for (const auto& index : indicesPOIs) {
            resultado.ruta.push_back("P" + std::to_string(index));
            resultado.puntajeTotal += pois[index].score; // Sumar el puntaje del POI
        }
    }

    // Agregar el último hotel
    resultado.ruta.push_back("H" + std::to_string(hotelesSeleccionados.back()));
    return resultado;
}



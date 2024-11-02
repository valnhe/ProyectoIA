#include <random>
#include <set>
#include <vector>
#include <iostream>
#include <cmath>
#include "instances.h"
#include "initialsolution.h"


// Función para calcular la distancia euclidiana entre hoteles o entre hoteles y POIs
double calcularDistancia(const Hotel& h1, const Hotel& h2) {
    return std::sqrt((h1.x - h2.x) * (h1.x - h2.x) + (h1.y - h2.y) * (h1.y - h2.y));
}

double calcularDistancia(const POI& p1, const POI& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double calcularDistancia(const Hotel& h1, const POI& p1) {
    return std::sqrt((p1.x - h1.x) * (p1.x - h1.x) + (p1.y - h1.y) * (p1.y - h1.y));
}

std::vector<int> seleccionarHotelesAleatorios(int D, const std::vector<Hotel>& hoteles, const std::vector<double>& Td) {
    std::vector<int> indicesSeleccionados;
    indicesSeleccionados.push_back(0); // Siempre incluir el hotel inicial H0

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, hoteles.size() - 1); // Desde H0 hasta el último hotel

    while (indicesSeleccionados.size() < D) {
        int nuevoHotel = distribucion(gen);
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

        if (cumpleCondicion) {
            indicesSeleccionados.push_back(nuevoHotel);
        }
    }

    indicesSeleccionados.push_back(1); // Siempre incluir el hotel final H1
    return indicesSeleccionados;
}

std::pair<std::vector<POI>, std::vector<int>> seleccionarPOIs(const Hotel& h1, const Hotel& h2, double Td, const std::vector<POI>& poisDisponibles, std::set<int>& indicesSeleccionados) {
    std::vector<POI> poisSeleccionados;
    std::vector<int> indicesSeleccionadosPOIs;
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(0, poisDisponibles.size() - 1);

    double distanciaTotal = 0.0;
    int contadorPOIs = 0;

     while (true) {
        if (poisDisponibles.empty()) break; // Verifica que haya POIs disponibles
        int indicePOI = distribucion(gen);

        // Verificar si el POI ya fue seleccionado
        if (indicesSeleccionados.find(indicePOI) == indicesSeleccionados.end()) {
            POI nuevoPOI = poisDisponibles[indicePOI];
            double distanciaDesdeUltimo = (poisSeleccionados.empty()) ? 
                                           calcularDistancia(h1, nuevoPOI) : 
                                           calcularDistancia(poisSeleccionados.back(), nuevoPOI);
            
             if (distanciaTotal + distanciaDesdeUltimo <= Td) {
                double distanciaAlFinal = calcularDistancia(h2, nuevoPOI);
                if (distanciaTotal + distanciaDesdeUltimo + distanciaAlFinal <= Td) {
                    poisSeleccionados.push_back(nuevoPOI);
                    indicesSeleccionados.insert(indicePOI);
                    indicesSeleccionadosPOIs.push_back(indicePOI);
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

    return {poisSeleccionados, indicesSeleccionadosPOIs};
}

SolucionInicial generarSolucionInicial(const std::vector<Hotel>& hoteles, const std::vector<POI>& poisDisponibles, const std::vector<double>& Td, int D) {
    std::vector<int> hotelesSeleccionados = seleccionarHotelesAleatorios(D, hoteles, Td);

    std::set<int> indicesSeleccionados;

    SolucionInicial resultado;
    resultado.puntajeTotal = 0.0;

    // Iterar sobre los pares de hoteles seleccionados
    for (size_t i = 0; i < hotelesSeleccionados.size() - 1; ++i) {
        Hotel h1 = hoteles[hotelesSeleccionados[i]];
        Hotel h2 = hoteles[hotelesSeleccionados[i + 1]];
        double td = Td[i];


        // Seleccionar POIs entre h1 y h2
        auto [poisEntre, indicesPOIs] = seleccionarPOIs(h1, h2, td, poisDisponibles, indicesSeleccionados);

        // Agregar hotel y POIs al resultado final
        resultado.ruta.push_back("H" + std::to_string(hotelesSeleccionados[i]));
        for (const auto& index : indicesPOIs) {
            resultado.ruta.push_back("P" + std::to_string(index)); // +1 para que sea 1-indexado
            resultado.puntajeTotal += poisDisponibles[index].score; // Sumar el puntaje del POI
        }
    }

    // Agregar el último hotel
    resultado.ruta.push_back("H" + std::to_string(hotelesSeleccionados.back()));
    
    return resultado;
}

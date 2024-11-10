# Proyecto de Inteligencia Artificial - OPHS

Este proyecto (aún en construcción) implementa una solución para el Orienteering Tour Problem With Hotel Selection en C++ a través de Hill Climbing con Mejor Mejora. A continuación, se detallan las instrucciones para la compilación y ejecución del proyecto.

## Estructura del Proyecto

- `instancias/`: Carpeta que contiene las instancias de prueba en formato `.ophs`.
- `funcionesAuxiliares.cpp`: Archivo de código fuente con funciones auxiliares.
- `hc.cpp`: Archivo de código fuente para la heurística.
- `main.cpp`: Archivo de código fuente principal.
- `randomSolution.cpp`: Archivo de código fuente para generar soluciones aleatorias.
- `readInstance.cpp`: Archivo de código fuente para leer instancias.
- `Makefile`: Archivo para gestionar la compilación y limpieza del proyecto.

## Instrucciones de Compilación
Para compilar el proyecto, se debe estar en el directorio raíz del proyecto y ejecute el siguiente comando:
```sh
make
```
Este comando compilará los archivos fuente y generará un ejecutable llamado proyecto.

## Ejecución del Proyecto
Para ejecutar el proyecto, utilice el siguiente formato de comando:
```sh
./proyecto nombreinstancia.ophs
```
### Ejemplo de Ejecución
Si desea ejecutar una de las instancias de prueba que se encuentran en la carpeta **instancias**, se puede hacer de la siguiente manera:
```sh
./proyecto instancias/64-80-10-4.ophs
```
## Instrucciones de Limpieza
Para eliminar los archivos compilados y dejar el directorio limpio, ejecute:
```sh
make clean
```

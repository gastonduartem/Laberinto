# Laberinto
# Generador y Solucionador de Laberintos

Este proyecto implementa un generador de laberintos aleatorios con callejones sin salida (ocultos) usando recursión (DFS) y un solucionador automático que halla el camino más corto desde la entrada hasta la salida usando BFS.

---

## Descripción

- **Generación de laberiento**  
  - Se crea una matriz de `alto × ancho` llena de muros (`#`), se marca la entrada en la esquina superior izquierda (`E`) y la salida en la esquina inferior derecha (`S`).  
  - A partir de `(0,0)`, se excavan pasillos (marca como espacio en blanco ` `) usando un algoritmo de _Depth-First Search_ recursivo.  
  - Cada vez que consideramos abrir una celda, comprobamos cuántos “vecinos pasillo” ya tiene; si tuviera más de 1, se omite para evitar crear bucles.  
  - El resultado es un laberinto “perfecto”: no hay ciclos y hay callejones sin salida.

- **Resolución automática**  
  - Una vez generado el laberinto, se aplica _Breadth-First Search_ (BFS) partiendo de la celda de entrada `E`.  
  - Se mantiene una cola que expande “en capas” las posiciones alcanzables, marcando cada celda visitada y guardando un predecesor para reconstruir la ruta.  
  - Al encontrar `S`, se reconstruye el camino óptimo de pasos ortogonales y se marca con `.` cada celda del recorrido (excepto `E` y `S`).

---

## Requisitos

- Compilador C++ compatible con **C++17** (por ejemplo, `g++` >= 7.0).  
- Entorno de consola / terminal (Linux, macOS o Windows con MinGW/Cygwin).  
- Ninguna biblioteca externa adicional: todo el código utiliza solo la STL de C++.

---

## Estructura de archivos

- `laberinto.cpp`  
  Contiene toda la lógica:  
  - Funciones para generar el laberinto (`crear_laberinto_vacio`, `tallar_caminos`, `generar_laberinto`).  
  - Función para resolver con BFS (`resolver_laberinto_BFS`).  
  - Función para imprimir (con borde) el laberinto antes y después de resolverlo.  
  - `main()` que pide las dimensiones al usuario, genera, resuelve e imprime.

- `README.md`  
  (Este documento) Explica cómo compilar, ejecutar y qué algoritmos se usan.

---

## Compilación y ejecución

1. **Abrir terminal** y navegar hasta la carpeta que contiene `laberinto.cpp`.  
2. Compilar con:
   ```bash
   g++ -std=c++17 laberinto.cpp -o laberinto

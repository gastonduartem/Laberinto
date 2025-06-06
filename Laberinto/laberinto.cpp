#include <iostream>  // Incluye la libreria de entrada/salida por consola
#include <vector> // Permite un arreglo dinamico de tamaño variable
#include <string> // Permite cadenas de caracteres
#include <random> // Permite numero aleatorios
#include <algorithm>  // Permite funciones de algoritmos
#include <utility> // Permite agrupar dos valores en un solo objeto
#include <queue> 

const char muro = '#';
const char camino = ' ';
const char entrada = 'E';
const char salida = 'S';

// Creacion del laberinto vacio lleno de muros
    // Crea un tablero vacio, marcando entrada y salida
std::vector<std::vector<char>> crear_laberinto_vacio(int numero_filas, int numero_columnas) {
    // Inicializa todas las celdas como muros
    std::vector<std::vector<char>> laberinto (numero_filas, std::vector<char>(numero_columnas, muro));

    //Marcamos la esquina superior izquierda como entrada, y el diagonal opuesto como salida
    laberinto[0][0] = entrada;
    laberinto[numero_filas - 1][numero_columnas - 1] = salida;

    return laberinto;
}

void imprimirLaberintoConBorde(const std::vector<std::vector<char>>& laberinto,
    int numeroFilas, int numeroColumnas) {

    //  Fila superior: "+" + numeroColumnas veces "-" + "+"
    std::cout << '+';
    for (int c = 0; c < numeroColumnas; ++c) {
        std::cout << '-';
    }
    std::cout << '+' << '\n';

    // 2) Filas centrales: "|" + contenido de cada fila + "|"
    for (int fila = 0; fila < numeroFilas; ++fila) {
        std::cout << '|';
        for (int columna = 0; columna < numeroColumnas; ++columna) {
            std::cout << laberinto[fila][columna];
        }
        std::cout << '|' << '\n';
    }

    // 3) Fila inferior: idéntica a la superior
    std::cout << '+';
    for (int c = 0; c < numeroColumnas; ++c) {
        std::cout << '-';
    }
    std::cout << '+' << '\n';
}

// Contamos cuantos vecinos de cada celda son ya estan marcadas como pasillo, sirve para evitar bucles
int contar_vecinos_pasillos (const std::vector<std::vector<char>>& laberinto, int fila, int columna) {

    int total_filas = laberinto.size();
    int total_columnas = laberinto[0].size();
    int contador = 0;

    // Desplazamiento posible de cada vecino
    const int delta_fila[4] = {-1, +1, 0, 0};
    const int delta_columna[4] = {0, 0, -1, +1};

    for (int i = 0; i<4; ++i) {
        int fila_vecina = fila + delta_fila[i];
        int columna_vecina = columna + delta_columna[i];
        
        // Verificamos que este dentro de los limites
        if (fila_vecina >= 0 && fila_vecina < total_filas && columna_vecina >= 0 && columna_vecina < total_columnas) {

            // Si la celda ya es pasillo, la contamos
            if (laberinto[fila_vecina][columna_vecina] == camino) {
                ++contador;
            }
        }
    }

    return contador;
}

// Marcamos la celda actual como camino y se barajan las 4 direcciones, y para cada vecino solo convertimos a camino si sigue siendo muro, y dp de convertir, que solo tenga un vecino maximo como camino

void tallar_caminos(int fila_actual, int columna_actual, std::vector<std::vector<char>>& laberinto, std::mt19937& motor_aleatorio) {

    int total_filas = laberinto.size();
    int total_columnas = laberinto[0].size();

    // Convertimos la celda actual en camino
    laberinto[fila_actual][columna_actual] = camino;

    // Definimos los movimientos posibles
    std::vector<std::pair<int,int>> direcciones = {
        {-1, 0}, {+1, 0}, {0, -1}, {0, +1}
    };

    // Mezclamos las direcciones posibles dentro de DIRECCIONES
    std::shuffle(direcciones.begin(), direcciones.end(), motor_aleatorio);

    // Recorremos cada direccion de mezclada
    for (auto& mezcla_direcciones : direcciones) {

        int fila_vecina = fila_actual + mezcla_direcciones.first;
        int columna_vecina = columna_actual + mezcla_direcciones.second;

        // Verficicamos sea dentro del laberinto, y se salta si la celda vecina sigue siendo muro
        if (fila_vecina < 0 || fila_vecina >= total_filas || columna_vecina < 0 || columna_vecina >= total_columnas) {
            continue;
        }

        if (laberinto[fila_vecina][columna_vecina] != muro) {
            continue;
        }

        // Evitamos bucels, solo abrimos si al hacerlo no habria mas de 1 vecino pasillo
        if (contar_vecinos_pasillos(laberinto, fila_vecina, columna_vecina) > 1) {
            continue;
        }

        // Si pasa todos los requisitos, llamamos recursivamente desde esa casilla vecina
        tallar_caminos(fila_vecina, columna_vecina, laberinto, motor_aleatorio);
    }
}

std::vector<std::vector<char>> generar_laberinto (int numero_filas, int numero_columnas) {

    // Creamos la matriz vacia llena de muros
    auto laberinto = crear_laberinto_vacio(numero_filas, numero_columnas);

    // Configuramos el motor aleatorio
    std::random_device numero_azar;
    std::mt19937 motor_aleatorio(numero_azar());

    // Abrimos caminos recursivamente desde la esquina superior izquierda
    tallar_caminos(0, 0, laberinto, motor_aleatorio);

    // Marcamos la entrada y la salida
    laberinto[0][0] = entrada;
    laberinto[numero_filas -1][numero_columnas -1] = salida;

    return laberinto;
}

// Resolucion del laberinto
std::vector<std::pair<int,int>> resolver_laberinto (std::vector<std::vector<char>>& laberinto) {

    int total_filas = laberinto.size();
    int total_columnas = laberinto[0].size();

    // Inicializamos filas y columnas con -1 para indicar “no encontrada"
    int fila_entrada = -1, columna_entrada = -1;

    // Recorremos cada fila y columna de la matriz
    for (int fila = 0; fila < total_filas; ++fila) {
        for (int columna = 0; columna < total_columnas; ++columna) {
            // Si en esa casilla encontramos la E, guardamos y rompemos ciclo
            if (laberinto[fila][columna] == entrada) {
                fila_entrada = fila;
                columna_entrada = columna;
                break;
            }
        }

        // Despues de romper el bucle de columnas, comprobamos tb si es que fila_entrada ya cambio de valor (Es de cir si ya encontramos E)
        if (fila_entrada != -1) {
            break;
        }

    }

    // Arrancamos con el BFS para resolver
    std::queue<std::pair<int,int>> cola_de_posiciones;
    std::vector<std::vector<bool>> casilla_visitada (total_filas, std::vector<bool>(total_columnas, false));

    // Matriz de predecesores para reconstruir el camino
    std::vector<std::vector<std::pair<int,int>>> predecesor (total_filas, std::vector<std::pair<int,int>> (total_columnas, {-1, -1}));

    // Metemos la entrada a la lista y la marcamos como visitada
    cola_de_posiciones.push({fila_entrada, columna_entrada});
    casilla_visitada[fila_entrada][columna_entrada] = true;

    // Desplazamientos validos
    const int despla_fila[4] = {-1, +1, 0, 0};
    const int despla_columna[4] = {0, 0, -1, +1};

    // Guardamos la posicion de salida cuando la encontremos
    std::pair<int, int> posicion_salida = {-1, -1};

    // Bucle de resolucion
    while (!cola_de_posiciones.empty()) {

        // Sacamos la primera posicion de la cola
        auto [fila_actual, columna_actual] = cola_de_posiciones.front();
        cola_de_posiciones.pop();

        // Si la casilla actual es la salida, guardamos y rompemos ciclo
        if (laberinto[fila_actual][columna_actual] == salida) {
            posicion_salida = {fila_actual, columna_actual};
            break;
        }

        // Recorremos los vecinos de la casilla actual
        for (int i=0; i<4; ++i) {
            int fila_vecina = fila_actual + despla_fila[i];
            int columna_vecina = columna_actual +  despla_columna[i];

            // Comprobamos que este dentro del tablero
            if (fila_vecina < 0 || fila_vecina >= total_filas || columna_vecina < 0 || columna_vecina >= total_columnas) {
                continue;
            }

            // Si la casilla vecina no es muro y no esta visitada, agregamos a la fila
            if (!casilla_visitada[fila_vecina][columna_vecina] && laberinto[fila_vecina][columna_vecina] != muro) {

                // Marcamos como visitada para no repetir en el futuro
                casilla_visitada[fila_vecina][columna_vecina] = true;

                // Guardamos que llegamos a esta vecina desde fila_actual y columna_actual
                predecesor[fila_vecina][columna_vecina] = {fila_actual, columna_actual};

                // Agregamos a la lista para procesar dp
                cola_de_posiciones.push({fila_vecina, columna_vecina});
            }
        }
    }

    // Si no encontramos salida, devolvemos vacio
    if (posicion_salida.first == -1) {
        return {};
    }

    // Reconstruimos el camino desde la salida hasta la entrada
    std::vector<std::pair<int,int>> camino_reverso;
    auto actual = posicion_salida;
    while (!(actual.first == fila_entrada && actual.second == columna_entrada)) {
        camino_reverso.push_back(actual);
        actual = predecesor[actual.first][actual.second];
    }

    // Agregamos la entrada al final
    camino_reverso.push_back({fila_entrada, columna_entrada});

    // Invertimos el vector para tener de entrada a salida
    std::reverse(camino_reverso.begin(), camino_reverso.end());

    return camino_reverso;

}




// Inicio del programa, funcion principal
int main() { 

    std::cout << "Bienvenidos al laberinto! \n";

    int alto, ancho;

    std::cout << "Introduzca el numero de filas \n";
    std::cin >> alto;

    std::cout << "Introduzca el numero de columnas \n";
    std::cin >> ancho;

    std::cout << "Dimensiones: " << ancho << "x" << alto << "\n";

    // Generamos el laberinto (sin resolver)
    auto laberinto = generar_laberinto(alto, ancho);
    imprimirLaberintoConBorde(laberinto, alto, ancho);
    
    std::cout<< "\n";

    // Resolvemos el laberinto
    auto ruta_optima = resolver_laberinto(laberinto);

    // Si la ruta esta vacia, no hay solucion
    if (ruta_optima.empty()) {
        std::cout << "No se encontro ningun camino desde la entrada hasta la salida \n";
    } else {
        // Marcamos el camino con puntos "."
        for (auto& coordenada : ruta_optima) {
            int fila = coordenada.first;
            int columna = coordenada.second;
            if (laberinto[fila][columna] != entrada && laberinto[fila][columna] != salida) {
                laberinto[fila][columna] = '.';
            }
        }
    }

    // Imprimir tablero resuelto
    std::cout << "Laberinto resuelto con '.' para el camino optimo \n";
    imprimirLaberintoConBorde(laberinto, alto, ancho);
    return 0;
}
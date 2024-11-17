#include <iostream>
#include <array>
#include <fstream>
#include <cctype> // Para funciones como isalpha
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cctype>
#include <stdexcept>
#include <chrono>

const int ALPHABET_SIZE = 26;
std::array<int, ALPHABET_SIZE> cost_delete_table;
std::array<std::array<int, ALPHABET_SIZE>, ALPHABET_SIZE> cost_transpose_matrix;
std::array<int, ALPHABET_SIZE> cost_insert_table;
std::array<std::array<int, ALPHABET_SIZE>, ALPHABET_SIZE> cost_matrix;



void cargarTablaCostosEliminacion(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    // Leer los valores de la tabla desde el archivo
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (!(archivo >> cost_delete_table[i])) {
            std::cerr << "Error al leer la tabla de costos de eliminación." << std::endl;
            archivo.close();
            return;
        }
    }

    archivo.close();
    std::cout << "Tabla de costos de eliminación cargada correctamente desde '" << nombreArchivo << "'." << std::endl;
}

void cargarMatrizCostosTransposicion(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    // Leer los valores de la matriz desde el archivo
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            if (!(archivo >> cost_transpose_matrix[i][j])) {
                std::cerr << "Error al leer la matriz de costos de transposición." << std::endl;
                archivo.close();
                return;
            }
        }
    }

    archivo.close();
    std::cout << "Matriz de costos de transposición cargada correctamente desde '" << nombreArchivo << "'." << std::endl;
}

void cargarTablaCostos(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    // Leer los valores de la tabla desde el archivo
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (!(archivo >> cost_insert_table[i])) {
            std::cerr << "Error al leer la tabla de costos." << std::endl;
            archivo.close();
            return;
        }
    }

    archivo.close();
    std::cout << "Tabla de costos cargada correctamente desde '" << nombreArchivo << "'." << std::endl;
}


void cargar_matriz_costos(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo de costos");
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            file >> cost_matrix[i][j];
        }
    }
    file.close();
}

int costo_sub(char a, char b) {
    int index_a = tolower(a) - 'a';
    int index_b = tolower(b) - 'a';

    // Validar que los índices estén en el rango del alfabeto
    if (index_a < 0 || index_a >= ALPHABET_SIZE || index_b < 0 || index_b >= ALPHABET_SIZE) {
        throw std::invalid_argument("Caracteres fuera del rango permitido");
    }

    return cost_matrix[index_a][index_b];
}
// Función que devuelve el costo de insertar el carácter 'b' en la cadena
int cost_ins(char b) {
    if (!std::isalpha(b)) {
        std::cerr << "El carácter '" << b << "' no es una letra válida." << std::endl;
        return -1;
    }

    b = std::tolower(b); // Convertir a minúscula si es necesario
    int index = b - 'a'; // Calcular el índice correspondiente
    return cost_insert_table[index];
}
// Función que devuelve el costo de intercambiar dos caracteres adyacentes
int cost_trans(char a, char b) {
    if (!std::isalpha(a) || !std::isalpha(b)) {
        std::cerr << "Uno o ambos caracteres no son letras válidas." << std::endl;
        return -1;
    }

    a = std::tolower(a); // Convertir a minúscula si es necesario
    b = std::tolower(b); // Convertir a minúscula si es necesario
    int index_a = a - 'a';
    int index_b = b - 'a';

    return cost_transpose_matrix[index_a][index_b];
}
int cost_del(char a) {
    if (!std::isalpha(a)) {
        std::cerr << "El carácter '" << a << "' no es una letra válida." << std::endl;
        return -1;
    }

    a = std::tolower(a); // Convertir a minúscula si es necesario
    int index = a - 'a'; // Calcular el índice correspondiente
    return cost_delete_table[index];
}

int minEditDistance(const std::string& A, const std::string& B) {
    int m = A.size();
    int n = B.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, INT_MAX));

    // Inicialización de la primera fila y columna
    dp[0][0] = 0;
    for (int i = 1; i <= m; ++i) {
        dp[i][0] = dp[i - 1][0] + cost_del(A[i - 1]);
    }
    for (int j = 1; j <= n; ++j) {
        dp[0][j] = dp[0][j - 1] + cost_ins(B[j - 1]);
    }

    // Llenado de la matriz dp
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            // Costo de inserción
            dp[i][j] = std::min(dp[i][j], dp[i][j - 1] + cost_ins(B[j - 1]));

            // Costo de eliminación
            dp[i][j] = std::min(dp[i][j], dp[i - 1][j] + cost_del(A[i - 1]));

            // Costo de sustitución
            dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1] + costo_sub(A[i - 1], B[j - 1]));

            // Costo de transposición
            if (i > 1 && j > 1 && A[i - 1] == B[j - 2] && A[i - 2] == B[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + cost_trans(A[i - 1], A[i - 2]));
            }
        }
    }

    return dp[m][n];
}



int main() {
    try {
        // Cargar las tablas y matrices de costos
        cargar_matriz_costos("cost_replace.txt");
        cargarTablaCostos("cost_insert.txt");
        cargarTablaCostosEliminacion("cost_delete.txt");
        cargarMatrizCostosTransposicion("cost_transpose.txt");


        // Cadenas de prueba
        std::string A = "abacadaeafagaha";
        std::string B = "ababacadaeafagh";
        auto inicio = std::chrono::high_resolution_clock::now();

        // Calcular y mostrar la distancia mínima de edición
        int result = minEditDistance(A, B);
        std::cout << "Distancia mínima de edición: " << result << std::endl;
        auto fin = std::chrono::high_resolution_clock::now();

        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);

        std::cout << "El código tardó: " << duracion.count() << " microsegundos." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


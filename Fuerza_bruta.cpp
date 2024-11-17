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




// Función de distancia mínima de edición con enfoque de fuerza bruta (recursión decreciente)
int minEditDistance(const std::string& A, const std::string& B, int i, int j) {
    // Caso base: si una de las cadenas está vacía
    if (i == -1) {
        int total_cost = 0;
        for (int k = j; k >= 0; --k) {  // Recorrer B desde j hasta 0
            total_cost += cost_ins(B[k]);
        }
        return total_cost;
    }
    if (j == -1) {
        int total_cost = 0;
        for (int k = i; k >= 0; --k) {  // Recorrer A desde i hasta 0
            total_cost += cost_del(A[k]);
        }
        return total_cost;
    }

    // Caso recursivo: pruebas para cada operación (inserción, eliminación, sustitución, transposición)
    int cost_insert = minEditDistance(A, B, i, j - 1) + cost_ins(B[j]);
    int cost_delete = minEditDistance(A, B, i - 1, j) + cost_del(A[i]);
    int cost_substitute = minEditDistance(A, B, i - 1, j - 1) + costo_sub(A[i], B[j]);

    int cost_transpose = INT_MAX;
    if (i - 1 >= 0 && j - 1 >= 0 && A[i] == B[j - 1] && A[i - 1] == B[j]) {
        cost_transpose = minEditDistance(A, B, i - 2, j - 2) + cost_trans(A[i], B[j]);
    }

    // Retornar el mínimo de las operaciones
    return std::min({cost_insert, cost_delete, cost_substitute, cost_transpose});
}

int main() {
    cargar_matriz_costos("cost_replace.txt");
    cargarTablaCostos("cost_insert.txt");
    cargarTablaCostosEliminacion("cost_delete.txt");
    cargarMatrizCostosTransposicion("cost_transpose.txt");
    std::string A = "abacadaeafagaha";
    std::string B = "ababacadaeafagh";

    auto inicio = std::chrono::high_resolution_clock::now();

    // Calcular la distancia mínima de edición con fuerza bruta
    int result = minEditDistance(A, B, A.size() - 1, B.size() - 1);
    std::cout << "Distancia mínima de edición (Fuerza Bruta): " << result << std::endl;
    auto fin = std::chrono::high_resolution_clock::now();

    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);

    std::cout << "El código tardó: " << duracion.count() << " microsegundos." << std::endl;

    return 0;
}

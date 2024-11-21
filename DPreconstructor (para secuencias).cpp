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
#include <utility>

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


int minEditDistance(const std::string& A, const std::string& B, std::vector<std::string>& operaciones) {
    int m = A.size();
    int n = B.size();

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, INT_MAX));
    std::vector<std::vector<std::string>> operacion(m + 1, std::vector<std::string>(n + 1, ""));

    // Inicialización de la primera fila y columna
    dp[0][0] = 0;
    for (int i = 1; i <= m; ++i) {
        dp[i][0] = dp[i - 1][0] + cost_del(A[i - 1]);
        operacion[i][0] = "Eliminar " + std::string(1, A[i - 1]);
    }
    for (int j = 1; j <= n; ++j) {
        dp[0][j] = dp[0][j - 1] + cost_ins(B[j - 1]);
        operacion[0][j] = "Insertar " + std::string(1, B[j - 1]);
    }

    // Llenado de la matriz dp y operacion
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            // Costo de inserción
            int cost_insert = dp[i][j - 1] + cost_ins(B[j - 1]);
            if (cost_insert < dp[i][j]) {
                dp[i][j] = cost_insert;
                operacion[i][j] = "Insertar " + std::string(1, B[j - 1]);
            }

            // Costo de eliminación
            int cost_delete = dp[i - 1][j] + cost_del(A[i - 1]);
            if (cost_delete < dp[i][j]) {
                dp[i][j] = cost_delete;
                operacion[i][j] = "Eliminar " + std::string(1, A[i - 1]);
            }

            // Costo de sustitución
            int cost_substitute = dp[i - 1][j - 1] + costo_sub(A[i - 1], B[j - 1]);
            if (cost_substitute < dp[i][j]) {
                dp[i][j] = cost_substitute;
                operacion[i][j] = "Sustituir " + std::string(1, A[i - 1]) + " por " + std::string(1, B[j - 1]);
            }

            // Costo de transposición
            if (i > 1 && j > 1 && A[i - 1] == B[j - 2] && A[i - 2] == B[j - 1]) {
                int cost_transpose = dp[i - 2][j - 2] + cost_trans(A[i - 1], A[i - 2]);
                if (cost_transpose < dp[i][j]) {
                    dp[i][j] = cost_transpose;
                    operacion[i][j] = "Transponer " + std::string(1, A[i - 2]) + std::string(1, A[i - 1]);
                }
            }
        }
    }

    // Reconstruir las operaciones
    int i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && dp[i][j] == dp[i - 1][j] + cost_del(A[i - 1])) {
            operaciones.push_back("Eliminar '" + std::string(1, A[i - 1]) + "' en posición " + std::to_string(i - 1));
            --i;
        } else if (j > 0 && dp[i][j] == dp[i][j - 1] + cost_ins(B[j - 1])) {
            operaciones.push_back("Insertar '" + std::string(1, B[j - 1]) + "' en posición " + std::to_string(j - 1));
            --j;
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + costo_sub(A[i - 1], B[j - 1])) {
            if (A[i - 1] != B[j - 1]) {
                operaciones.push_back("Sustituir '" + std::string(1, A[i - 1]) + "' por '" + std::string(1, B[j - 1]) + "' en posición " + std::to_string(i - 1));
            }
            --i;
            --j;
        } else if (i > 1 && j > 1 && dp[i][j] == dp[i - 2][j - 2] + cost_trans(A[i - 1], A[i - 2]) &&
                A[i - 1] == B[j - 2] && A[i - 2] == B[j - 1]) {
            operaciones.push_back("Transponer '" + std::string(1, A[i - 2]) + "' y '" + std::string(1, A[i - 1]) + "' en posiciones " + std::to_string(i - 2) + " y " + std::to_string(i - 1));
            i -= 2;
            j -= 2;
        }
    }


    // Invertir las operaciones, ya que las hemos recolectado de atrás hacia adelante
    std::reverse(operaciones.begin(), operaciones.end());

    return dp[m][n];
}

int main() {
    try {
        cargar_matriz_costos("cost_replace.txt");
        cargarTablaCostos("cost_insert.txt");
        cargarTablaCostosEliminacion("cost_delete.txt");
        cargarMatrizCostosTransposicion("cost_transpose.txt");


        // Cadenas de prueba
        std::string A = "ababababab";
        std::string B = "ababacabab";

        // Resultado de la distancia mínima de edición
        std::vector<std::string> operaciones;
        int result = minEditDistance(A, B, operaciones);
        std::cout << "Distancia mínima de edición: " << result << std::endl;

        // Mostrar las operaciones realizadas
        std::cout << "Operaciones realizadas:" << std::endl;
        for (const std::string& op : operaciones) {
            std::cout << op << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

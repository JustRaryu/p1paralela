#include <iostream>
#include <chrono>
#include <omp.h>


using namespace std;
int main(int argc, char *argv[]) {
    srand (time(NULL));

    int rows = atoi(argv[1]);
    int cols = rows;
    int nth = atoi(argv[2]);

    // Paso 1: Pide memoria dinámica para un arreglo de punteros a filas
    // esto usa una representacion 2D
    int** matrix = new int*[rows];
    int** m2 = new int*[rows];
    int** mult = new int*[rows];

    // Paso 2: Pide memoria para cada arreglo de fila
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
        m2[i] = new int[cols];
        mult[i] = new int[cols];
    }

    // Paso 3: Inicializa los valores de la matrix
    auto start = chrono::steady_clock::now();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
            m2[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mult[i][j] = 0;
        }
    }

    auto end = chrono::steady_clock::now();
    auto tinitns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout<<" tiempo inicializacion "<<tinitns<<" ns "<< tinitns/1000000000.0<<" s"<< endl;


    // Multiplicacion de matrices, guardando la columna de la segunda en un vector
    omp_set_num_threads(nth);
    #pragma omp parallel for
    for (int j = 0; j < cols; j++) {
        // Save the entire column of m2 into a temporary vector
        int* column = new int[rows];
        for (int k = 0; k < rows; k++) {
            column[k] = m2[k][j];
        }

        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                mult[i][j] += matrix[i][k] * column[k];
            }
        }

        delete[] column;
    }

    // idea: pedir toda la columna y guardarla en un vector auxiliar para que sea mas facil accederla

    // Paso 4: Imprime por pantalla la matriz
    std::cout << "Matriz contiene :\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << std::endl;
    }

    std::cout << "Matriz 2 contiene :\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << m2[i][j] << " ";
        }
        cout << std::endl;
    }

    std::cout << "Mult contiene :\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << mult[i][j] << " ";
        }
        cout << std::endl;
    }

    // Paso 5: Libera memoria dinámica (primero cada fila, luego el arreglo matrix )
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i]; //libera memoria de cada fila
        delete[] m2[i];
        delete[] mult[i];
    }
    delete[] matrix; //libera la memoria de los punteros en matrix
    delete[] m2;
    delete[] mult;

    return 0;
}

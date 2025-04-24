#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <functional>
#include <numeric> 

#include "matrix_factory.h"
#include "matrix_multiplication.h"
#include "matrix_parallel_multiplication.h"


void print_matrix(Matrix matrix){
    for (auto row : matrix) {
        for (int number : row) {
            std::cout << number << " ";
        }
        std::cout << "\n" << std::flush;
    }
}

double mean(const std::vector<double>& vec) {
    if (vec.empty()) {
        return 0.0;
    }

    double total = std::accumulate(vec.begin(), vec.end(), 0);
    return total / vec.size();
}

double standar_desviation(const std::vector<double>& vec) {
    if (vec.empty()) {
        return 0.0;
    }

    double vector_mean = mean(vec);
    double square_total = 0.0;

    for (int value : vec) {
        square_total += std::pow(value - vector_mean, 2);
    }

    return std::sqrt(square_total / vec.size()); 
}

void test_function(size_t size, std::function<Matrix(Matrix, Matrix)> mult, MatrixFactory& factory) {
    int rep = 10;
    std::vector<double> time(rep, 0.0);

    for (size_t i = 0; i < rep; i++){
        Matrix A = factory.get_random_matrix(size, 0, 9);    
        Matrix B = factory.get_random_matrix(size, 0, 9);
        
        auto start = std::chrono::high_resolution_clock::now();
        Matrix C = mult(A, B);
        auto finish = std::chrono::high_resolution_clock::now();
        time[i] = std::chrono::duration_cast<std::chrono::milliseconds> (finish - start).count();
    }

    double time_mean = mean(time);
    double time_standar_desviation = standar_desviation(time);

    std::cout << "----------------------------" << std::endl;
    std::cout << "size: " << size << std::endl;
    std::cout << "mean[ms]: " << time_mean << std::endl;
    std::cout << "standar_desviation: " << time_standar_desviation<< std::endl;
    
}

int main(){
    
    MatrixFactory factory = MatrixFactory();


    std::cout << "[1] Secuencial     [2] Paralelizado" << std::endl;
    int tipo = 0;
    std::cin >> tipo;
    bool is_secuencial = tipo == 1 ? true : false;

    std::cout << "Elige un algoritmo: " << std::endl;
    std::cout << "[1] Tradicional      [2] Vector" << std::endl;
    std::cout << "[3] Traspuesta       [4] Bloque iterativo" << std::endl;
    std::cout << "[5] Bloque recursivo [6] Strassen" << std::endl;
    int algoritmo = 0;
    std::cin >> algoritmo;


    int rep = 10;    
    for (size_t potencia = 1; potencia < 10;potencia++) {
        size_t size = static_cast<int>(pow(2, potencia));

        if (is_secuencial){
            switch (algoritmo) {
            case 1:
                test_function(size, MatrixMult::tradicional, factory);
                break;
            
            case 2:
                test_function(size, MatrixMult::vector, factory);
                break;
            case 3:
                test_function(size, MatrixMult::transpuesta, factory);
                break;
            case 4:
                test_function(size, MatrixMult::bloques_iterativo, factory);
                break;
            case 5:
                test_function(size, MatrixMult::bloques_recursivo, factory);
                break;
            case 6:
                test_function(size, MatrixMult::strassen, factory);
                break;
            default:
                break;
            }
        }
        else {
            switch (algoritmo) {
                case 1:
                    test_function(size, MatrixParallelMult::tradicional, factory);
                    break;
                
                case 2:
                    test_function(size, MatrixParallelMult::vector, factory);
                    break;
                case 3:
                    test_function(size, MatrixParallelMult::transpuesta, factory);
                    break;
                case 4:
                    test_function(size, MatrixParallelMult::bloques_iterativo, factory);
                    break;
                case 5:
                    test_function(size, MatrixParallelMult::bloques_recursivo, factory);
                    break;
                case 6:
                    test_function(size, MatrixParallelMult::strassen, factory);
                    break;
                default:
                    break;
                }
        }
    }

    return 0;
}
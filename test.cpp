// Header file for input output functions
#include <iostream>
#include "matrix_multiplication.h"
// main() function: where the execution of
// c++ program begins
void print_matrix(Matrix matrix){
    for (auto row : matrix) {
        for (int number : row) {
            std::cout << number << " ";
        }
        std::cout << "\n" << std::flush;
    }
}

int main(){
    Matrix A = {{3, 5, 0, 3}, {1, 7, 9, 2}, {8, 7, 8, 3}, {7, 7, 7, 7}};
    Matrix B = {{5, 8, 6, 8}, {0, 2, 6, 8}, {3, 8 , 1, 8}, {6, 3, 3, 4}};

    std::cout << "-----A-----" << std::endl;
    print_matrix(A);
    std::cout << "-----B-----" << std::endl;
    print_matrix(B);
    std::cout << "-----tradicional-----" << std::endl;
    print_matrix(MatrixMult::tradicional(A,B));
    std::cout << "-----traspuesta-----" << std::endl;
    print_matrix(MatrixMult::transpuesta(A,B));
    std::cout << "-----vector-----" << std::endl;
    print_matrix(MatrixMult::vector(A,B));

    std::cout << "-----block multiplication-----" << std::endl;
    print_matrix(MatrixMult::bloques_recursivo(A,B));
    std::cout << "-----strassen multiplication-----" << std::endl;
    print_matrix(MatrixMult::strassen(A,B));

    return 0;
}
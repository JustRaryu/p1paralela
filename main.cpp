#include <iostream>
#include <vector>

#include "matrix_factory.h"
#include "matrix_multiplication.h"


void print_matrix(Matrix matrix){
    for (auto row : matrix) {
        for (int number : row) {
            std::cout << number << " ";
        }
        std::cout << "\n" << std::flush;
    }
}


int main(){
    
    MatrixFactory factory = MatrixFactory();
    size_t matrix_size = 0;
    std::cin >> matrix_size;

    Matrix matrix1 = factory.get_random_matrix(matrix_size, 0, 9);
    Matrix matrix2 = factory.get_random_matrix(matrix_size, 0, 9);

    return 0;
}
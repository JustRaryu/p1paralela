#include "matrix_factory.h"
#include <iostream>
#include <random>
#include <vector>

MatrixFactory::MatrixFactory() : gen(rd()) {}

Matrix MatrixFactory::get_random_matrix(size_t size, int min, int max) {
    std::uniform_int_distribution<> dist(min, max);

    Matrix matrix(size, std::vector<int>(size, 0));
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            matrix[i][j] = dist(this->gen);
        }
    }

    return matrix;
}
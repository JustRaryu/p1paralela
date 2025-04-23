#ifndef MATRIX_H
#define MATRIX_H

#include <random>
#include <vector>
#include <iostream>

typedef std::vector<std::vector<int>> Matrix;

class MatrixFactory{
    private:
        std::random_device rd;
        std::mt19937 gen;
    
    public:
        MatrixFactory();
        Matrix get_random_matrix(size_t size, int min, int max);

};

#endif
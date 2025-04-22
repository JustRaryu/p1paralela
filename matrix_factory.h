#ifndef MATRIX_H
#define MATRIX_H

#include <random>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

class MatrixFactory{
    private:
        std::random_device rd;
        std::mt19937 gen;
    
    public:
        MatrixFactory();
        Matrix get_random_matrix(int size, int min, int max);

};

#endif
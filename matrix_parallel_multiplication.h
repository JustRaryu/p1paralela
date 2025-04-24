#ifndef MATRIX_PARALLEL_MULT
#define MATRIX_PARALLEL_MULT

#include "matrix_factory.h"

class MatrixParallelMult {
    public:
        static Matrix tradicional(const Matrix& A, const Matrix& B);
        static Matrix vector(const Matrix& A, const Matrix& B);
        static Matrix transpuesta(const Matrix& A, const Matrix& B);
        static Matrix bloques_iterativo(const Matrix& A, const Matrix& B);
        static Matrix bloques_recursivo(const Matrix& A, const Matrix& B);
        static Matrix strassen(const Matrix& A, const Matrix& B);

    private:
        MatrixParallelMult() = delete;
        static Matrix inner_bloques_recursivo(const Matrix& A, const Matrix& B);
        static Matrix inner_strassen(const Matrix& A, const Matrix& B);
};

#endif
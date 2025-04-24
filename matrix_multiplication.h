#ifndef MATRIX_MULT
#define MATRIX_MULT

#include "matrix_factory.h"
#include <vector>

class MatrixMult {
  private:
        MatrixMult() = delete;
        static Matrix create_matrix(size_t size);
        static Matrix case_base(const Matrix& A, const Matrix& B);
        static std::vector<Matrix> get_sub_matrices(const Matrix& parent);
        static Matrix add(const Matrix& A, const Matrix& B);
        static Matrix subtract(const Matrix& A, const Matrix& B);
        static Matrix sub_matrices_to_matrix(const std::vector<Matrix> &sub_matrices, size_t size); 
        static Matrix inner_bloques_paralelos(const Matrix& A, const Matrix& B);
   public:
        static Matrix tradicional(const Matrix& A, const Matrix& B);
        static Matrix vector(const Matrix& A, const Matrix& B);
        static Matrix transpuesta(const Matrix& A, const Matrix& B);
        static Matrix bloques_iterativo(const Matrix& A, const Matrix& B);
        static Matrix bloques_recursivo(const Matrix& A, const Matrix& B);
        static Matrix strassen(const Matrix& A, const Matrix& B);
        static Matrix bloques_paralelos(const Matrix& A, const Matrix& B);
   };

#endif
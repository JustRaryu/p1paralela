#include "matrix_parallel_multiplication.h"
#include "matrix_multiplication.h"

#include <iostream>
#include <vector>
#include <omp.h>


Matrix MatrixParallelMult::tradicional(const Matrix& A, const Matrix& B) {
    Matrix result = MatrixMult::create_matrix(A.size());
    size_t size = result.size();

    #pragma omp parallel for collapse(2) shared(A, B, result)
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            for (size_t k = 0; k < size; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

Matrix MatrixParallelMult::transpuesta(const Matrix& A, const Matrix& B) {
    Matrix transpose = MatrixMult::create_matrix(A.size());
    size_t size = transpose.size();

    #pragma omp parallel for collapse(2) shared(B, transpose)
    for (size_t i = 0; i < size;i++) {
        for (size_t j = 0; j < size;j++) {
            transpose[j][i] = B[i][j];
        }
    }

    Matrix result = tradicional(A, transpose);

    return result;
}

Matrix MatrixParallelMult::vector(const Matrix& A, const Matrix& B) {
    Matrix result = MatrixMult::create_matrix(A.size());
    size_t size = result.size();

    #pragma omp parallel for shared(A, B, result, size)
    for (int j = 0; j < size; j++) {
        std::vector<int> aux(size, 0);
        for (int k = 0; k < size; k++) {
            aux[k] = B[k][j];
        }

        for (int i = 0; i < size; i++) {
            for (int k = 0; k < size; k++) {
                result[i][j] += A[i][k] * aux[k];
            }
        }
    }   

    return result;
}

Matrix MatrixParallelMult::bloques_iterativo(const Matrix& A, const Matrix& B) {
    size_t parent_size = A.size();
    size_t block_size = parent_size/2;

    Matrix result = MatrixMult::create_matrix(parent_size);

    #pragma omp parallel for collapse(2) shared(A, B, result, parent_size, block_size)
    for (int i = 0; i < parent_size; i += block_size) {
        for (int j = 0; j < parent_size; j += block_size) {
            for (int k = 0; k < parent_size; k += block_size) {
                for (int ii = i; ii < std::min(i + block_size, parent_size); ii++) {
                    for (int jj = j; jj < std::min(j + block_size, parent_size); jj++) {
                        for (int kk = k; kk < std::min(k + block_size, parent_size); kk++) {
                            result[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }

    return result;
}

Matrix MatrixParallelMult::bloques_recursivo(const Matrix& A, const Matrix& B) {
    Matrix result;
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            result = inner_bloques_recursivo(A,B);
        }
    }
    return result;
}

Matrix MatrixParallelMult::strassen(const Matrix& A, const Matrix& B) {
    Matrix result;
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            result = inner_strassen(A,B);
        }
    }
    return result;
}


Matrix MatrixParallelMult::inner_strassen(const Matrix& A, const Matrix& B){
    size_t size = A.size();
    
    if (size == 1) {
        return MatrixMult::case_base(A,B);
    }

    std::vector<Matrix> sub_A = MatrixMult::get_sub_matrices(A);
    std::vector<Matrix> sub_B = MatrixMult::get_sub_matrices(B);

    Matrix result;    
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::vector<Matrix> strassen_m(7, Matrix(size/2, std::vector<int>(size/2, 0)));

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[0] = inner_strassen(MatrixMult::add(sub_A[0], sub_A[3]), MatrixMult::add(sub_B[0], sub_B[3]));

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[1] = inner_strassen(MatrixMult::add(sub_A[2], sub_A[3]), sub_B[0]);

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[2] = inner_strassen(sub_A[0], MatrixMult::subtract(sub_B[1], sub_B[3]));

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[3] = inner_strassen(sub_A[3], MatrixMult::subtract(sub_B[2], sub_B[0]));

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[4] = inner_strassen(MatrixMult::add(sub_A[0], sub_A[1]), sub_B[3]);

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[5] = inner_strassen(MatrixMult::subtract(sub_A[2], sub_A[0]), MatrixMult::add(sub_B[0], sub_B[1]));

            #pragma omp task shared(strassen_m) firstprivate(sub_A, sub_B)
            strassen_m[6] = inner_strassen(MatrixMult::subtract(sub_A[1], sub_A[3]), MatrixMult::add(sub_B[2], sub_B[3]));

            // Espera a que todas las tareas terminen
            #pragma omp taskwait

            // Composición de las 4 sub-matrices resultado
            std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));
            results_sub_matrices[0] = MatrixMult::add(MatrixMult::subtract(MatrixMult::add(strassen_m[0], strassen_m[3]), strassen_m[4]), strassen_m[6]); 
            results_sub_matrices[1] = MatrixMult::add(strassen_m[2], strassen_m[4]); 
            results_sub_matrices[2] = MatrixMult::add(strassen_m[1], strassen_m[3]); 
            results_sub_matrices[3] = MatrixMult::add(MatrixMult::subtract(strassen_m[0], strassen_m[1]), MatrixMult::add(strassen_m[2], strassen_m[5])); 

            result = MatrixMult::sub_matrices_to_matrix(results_sub_matrices, size);
        }
    }

    return result;
}
Matrix MatrixParallelMult::inner_bloques_recursivo(const Matrix& A, const Matrix& B) {
    size_t size = A.size();
    if (size == 1) {
        return MatrixMult::case_base(A,B);
    }

    auto sub_A = MatrixMult::get_sub_matrices(A);
    auto sub_B = MatrixMult::get_sub_matrices(B);
    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2)));
    
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[0] = MatrixMult::add(bloques_recursivo(sub_A[0], sub_B[0]), bloques_recursivo(sub_A[1], sub_B[2]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[1] = MatrixMult::add(bloques_recursivo(sub_A[0], sub_B[1]), bloques_recursivo(sub_A[1], sub_B[3]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[2] = MatrixMult::add(bloques_recursivo(sub_A[2], sub_B[0]), bloques_recursivo(sub_A[3], sub_B[2]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[3] = MatrixMult::add(bloques_recursivo(sub_A[2], sub_B[1]), bloques_recursivo(sub_A[3], sub_B[3]));
    }
    #pragma omp taskwait

    return MatrixMult::sub_matrices_to_matrix(results_sub_matrices, size);
}




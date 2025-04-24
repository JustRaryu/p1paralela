#include "matrix_multiplication.h"

#include <iostream>
#include <vector>

Matrix MatrixMult::create_matrix(size_t size) {
    return Matrix(size, std::vector<int>(size, 0));
}

Matrix MatrixMult::tradicional(const Matrix& A, const Matrix& B) {
    Matrix result = create_matrix(A.size());

    size_t size = result.size();
    for (size_t i = 0; i < size;i++) {
        for (size_t j = 0; j < size;j++) {
            for (size_t k = 0; k < size;k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

Matrix MatrixMult::transpuesta(const Matrix& A, const Matrix& B) {
    Matrix transpose = create_matrix(A.size());
    
    size_t size = transpose.size();
    for (size_t i = 0; i < size;i++) {
        for (size_t j = 0; j < size;j++) {
            transpose[j][i] = B[i][j];
        }
    }

    Matrix result = tradicional(A, transpose);


    return result;
}

Matrix MatrixMult::vector(const Matrix& A, const Matrix& B) {
    Matrix result = create_matrix(A.size());

    size_t size = result.size();
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

Matrix MatrixMult::case_base(const Matrix& A, const Matrix& B) {
    return Matrix{std::vector<int>{A[0][0]*B[0][0]}};
}

std::vector<Matrix> MatrixMult::get_sub_matrices(const Matrix& parent) {
    size_t size = parent.size()/2;

    std::vector<Matrix> sub_matrices(4, Matrix(size, std::vector<int>(size,0)));
    for (size_t i = 0; i < size;i++) {
        for (size_t j = 0; j < size;j++) {
            sub_matrices[0][i][j] = parent[i][j];
            sub_matrices[1][i][j] = parent[i][j + size];
            sub_matrices[2][i][j] = parent[i + size][j];
            sub_matrices[3][i][j] = parent[i + size][j + size];
        }
    }

    return sub_matrices;
}

Matrix MatrixMult::add(const Matrix& A, const Matrix& B) {
    size_t size = A.size();

    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0;i < size;i++) {
        for (size_t j = 0;j < size;j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

Matrix MatrixMult::subtract(const Matrix& A, const Matrix& B) {
    size_t size = A.size();

    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0;i < size;i++) {
        for (size_t j = 0;j < size;j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

Matrix MatrixMult::sub_matrices_to_matrix(const std::vector<Matrix> &sub_matrices, size_t size) {
    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0; i < size/2;i++) {
        for (size_t j = 0; j < size/2;j++) {
            result[i][j] = sub_matrices[0][i][j];
            result[i][j + size/2] = sub_matrices[1][i][j];
            result[i + size/2][j] = sub_matrices[2][i][j];
            result[i + size/2][j + size/2] = sub_matrices[3][i][j];
        }
    }

    return result;
}

Matrix MatrixMult::bloques(const Matrix& A,const Matrix& B){
    size_t size = A.size();
    
    if (size == 1) {
        return case_base(A,B);
    }

    std::vector<Matrix> sub_A = get_sub_matrices(A);
    std::vector<Matrix> sub_B = get_sub_matrices(B);


    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));

    results_sub_matrices[0] = add(bloques(sub_A[0], sub_B[0]), bloques(sub_A[1], sub_B[2]));
    results_sub_matrices[1] = add(bloques(sub_A[0], sub_B[1]), bloques(sub_A[1], sub_B[3]));
    results_sub_matrices[2] = add(bloques(sub_A[2], sub_B[0]), bloques(sub_A[3], sub_B[2]));
    results_sub_matrices[3] = add(bloques(sub_A[2], sub_B[1]), bloques(sub_A[3], sub_B[3]));

    Matrix result = sub_matrices_to_matrix(results_sub_matrices, size);
    
    return result;

}

Matrix MatrixMult::strassen(const Matrix& A,const Matrix& B){
    size_t size = A.size();
    
    if (size == 1) {
        return case_base(A,B);
    }

    std::vector<Matrix> sub_A = get_sub_matrices(A);
    std::vector<Matrix> sub_B = get_sub_matrices(B);


    std::vector<Matrix> strassen_m(7, Matrix(size/2, std::vector<int>(size/2, 0)));

    strassen_m[0] = strassen(add(sub_A[0], sub_A[3]), add(sub_B[0], sub_B[3]));
    strassen_m[1] = strassen(add(sub_A[2], sub_A[3]), sub_B[0]);
    strassen_m[2] = strassen(sub_A[0], subtract(sub_B[1], sub_B[3]));
    strassen_m[3] = strassen(sub_A[3], subtract(sub_B[2], sub_B[0]));
    strassen_m[4] = strassen(add(sub_A[0], sub_A[1]), sub_B[3]);
    strassen_m[5] = strassen(subtract(sub_A[2], sub_A[0]), add(sub_B[0], sub_B[1]));
    strassen_m[6] = strassen(subtract(sub_A[1], sub_A[3]), add(sub_B[2], sub_B[3]));


    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));

    results_sub_matrices[0] = add(subtract(add(strassen_m[0], strassen_m[3]), strassen_m[4]), strassen_m[6]); 
    results_sub_matrices[1] = add(strassen_m[2], strassen_m[4]); 
    results_sub_matrices[2] = add(strassen_m[1], strassen_m[3]); 
    results_sub_matrices[3] = add(subtract(strassen_m[0], strassen_m[1]), add(strassen_m[2], strassen_m[5])); 

    Matrix result = sub_matrices_to_matrix(results_sub_matrices, size);
    
    return result;

}

Matrix MatrixMult::inner_bloques_paralelos(const Matrix& A, const Matrix& B) {
    size_t size = A.size();
    if (size == 1) {
        return case_base(A,B);
    }

    auto sub_A = get_sub_matrices(A);
    auto sub_B = get_sub_matrices(B);
    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2)));
    
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[0] = add(bloques(sub_A[0], sub_B[0]), bloques(sub_A[1], sub_B[2]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[1] = add(bloques(sub_A[0], sub_B[1]), bloques(sub_A[1], sub_B[3]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[2] = add(bloques(sub_A[2], sub_B[0]), bloques(sub_A[3], sub_B[2]));
    }
    #pragma omp task shared(results_sub_matrices) firstprivate(sub_A, sub_B)
    {
        results_sub_matrices[3] = add(bloques(sub_A[2], sub_B[1]), bloques(sub_A[3], sub_B[3]));
    }
    #pragma omp taskwait

    return sub_matrices_to_matrix(results_sub_matrices, size);
}


Matrix MatrixMult::bloques_paralelos(const Matrix& A, const Matrix& B) {
    Matrix result;
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            result = inner_bloques_paralelos(A,B);
        }
    }
    return result;
}

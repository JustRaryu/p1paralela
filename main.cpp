#include <iostream>
#include "matrix_factory.h"
#include <vector>

Matrix case_base(Matrix A, Matrix B) {
    return Matrix{std::vector<int>{A[0][0]*B[0][0]}};
}

std::vector<Matrix> get_sub_matrices(const Matrix& parent) {
    size_t size = parent.size()/2;

    std::vector<Matrix> sub_matrices(4, Matrix(size, std::vector<int>(size,0)));
    for (size_t i = 0; i < size;i++) {
        for (size_t j = 0; i < size;i++) {
            sub_matrices[0][i][j] = parent[i][j];
            sub_matrices[1][i][j] = parent[i][j + size];
            sub_matrices[2][i][j] = parent[i + size][j];
            sub_matrices[3][i][j] = parent[i + size][j + size];
        }
    }

    return sub_matrices;
}

Matrix add(const Matrix& A, const Matrix& B) {
    size_t size = A.size();

    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0;i < size;i++) {
        for (size_t j = 0;j < size;j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

Matrix multiplication(const Matrix& A,const Matrix& B){
    int size = A.size();
    
    if (size == 1) {
        return case_base(A,B);
    }

    std::vector<Matrix> sub_A = get_sub_matrices(A);
    std::vector<Matrix> sub_B = get_sub_matrices(B);


    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));

    results_sub_matrices[0] = add(multiplication(sub_A[0], sub_B[0]), multiplication(sub_A[1], sub_B[2]));
    results_sub_matrices[1] = add(multiplication(sub_A[0], sub_B[1]), multiplication(sub_A[1], sub_B[3]));
    results_sub_matrices[2] = add(multiplication(sub_A[2], sub_B[0]), multiplication(sub_A[3], sub_B[2]));
    results_sub_matrices[3] = add(multiplication(sub_A[2], sub_B[1]), multiplication(sub_A[3], sub_B[3]));

    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0; i < size/2;i++) {
        for (size_t j = 0; j < size/2;j++) {
            result[i][j] = results_sub_matrices[0][i][j];
            result[i][j + size/2] = results_sub_matrices[1][i][j];
            result[i + size/2][j] = results_sub_matrices[2][i][j];
            result[i + size/2][j + size/2] = results_sub_matrices[3][i][j];
        }
    }

    return result;

}

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

    print_matrix(matrix1);
    std::cout << "------" << std::endl;
    print_matrix(matrix2);
    std::cout << "------" << std::endl;
    print_matrix(multiplication(matrix1, matrix2));

    return 0;
}
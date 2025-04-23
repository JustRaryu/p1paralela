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
        for (size_t j = 0; j < size;j++) {
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

Matrix subtract(const Matrix& A, const Matrix& B) {
    size_t size = A.size();

    Matrix result(size, std::vector<int>(size, 0));
    for (size_t i = 0;i < size;i++) {
        for (size_t j = 0;j < size;j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

Matrix sub_matrices_to_matrix(const std::vector<Matrix> &sub_matrices, size_t size) {
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

Matrix block_mult(const Matrix& A,const Matrix& B){
    size_t size = A.size();
    
    if (size == 1) {
        return case_base(A,B);
    }

    std::vector<Matrix> sub_A = get_sub_matrices(A);
    std::vector<Matrix> sub_B = get_sub_matrices(B);


    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));

    results_sub_matrices[0] = add(block_mult(sub_A[0], sub_B[0]), block_mult(sub_A[1], sub_B[2]));
    results_sub_matrices[1] = add(block_mult(sub_A[0], sub_B[1]), block_mult(sub_A[1], sub_B[3]));
    results_sub_matrices[2] = add(block_mult(sub_A[2], sub_B[0]), block_mult(sub_A[3], sub_B[2]));
    results_sub_matrices[3] = add(block_mult(sub_A[2], sub_B[1]), block_mult(sub_A[3], sub_B[3]));

    Matrix result = sub_matrices_to_matrix(results_sub_matrices, size);
    
    return result;

}

Matrix strassen_mult(const Matrix& A,const Matrix& B){
    size_t size = A.size();
    
    if (size == 1) {
        return case_base(A,B);
    }

    std::vector<Matrix> sub_A = get_sub_matrices(A);
    std::vector<Matrix> sub_B = get_sub_matrices(B);


    std::vector<Matrix> strassen_m(7, Matrix(size/2, std::vector<int>(size/2, 0)));

    strassen_m[0] = strassen_mult(add(sub_A[0], sub_A[3]), add(sub_B[0], sub_B[3]));
    strassen_m[1] = strassen_mult(add(sub_A[2], sub_A[3]), sub_B[0]);
    strassen_m[2] = strassen_mult(sub_A[0], subtract(sub_B[1], sub_B[3]));
    strassen_m[3] = strassen_mult(sub_A[3], subtract(sub_B[2], sub_B[0]));
    strassen_m[4] = strassen_mult(add(sub_A[0], sub_A[1]), sub_B[3]);
    strassen_m[5] = strassen_mult(subtract(sub_A[2], sub_A[0]), add(sub_B[0], sub_B[1]));
    strassen_m[6] = strassen_mult(subtract(sub_A[1], sub_A[3]), add(sub_B[2], sub_B[3]));


    std::vector<Matrix> results_sub_matrices(4, Matrix(size/2, std::vector<int>(size/2, 0)));

    results_sub_matrices[0] = subtract(add(strassen_m[0], strassen_m[3]), add(strassen_m[4], strassen_m[6])); 
    results_sub_matrices[1] = add(strassen_m[2], strassen_m[4]); 
    results_sub_matrices[2] = add(strassen_m[1], strassen_m[3]); 
    results_sub_matrices[3] = add(subtract(strassen_m[0], strassen_m[1]), add(strassen_m[2], strassen_m[5])); 

    Matrix result = sub_matrices_to_matrix(results_sub_matrices, size);
    
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

void tests(){
    Matrix A = {{3, 5, 0, 3}, {1, 7, 9, 2}, {8, 7, 8, 3}, {7, 7, 7, 7}};
    Matrix B = {{5, 8, 6, 8}, {0, 2, 6, 8}, {3, 8 , 1, 8}, {6, 3, 3, 4}};

    std::cout << "-----A-----" << std::endl;
    print_matrix(A);
    std::cout << "-----B-----" << std::endl;
    print_matrix(B);
    std::cout << "-----add-----" << std::endl;
    print_matrix(add(A, B));
    std::cout << "-----subtract-----" << std::endl;
    print_matrix(subtract(A, B));
    std::cout << "-----sub matrices A-----" << std::endl;
    auto sub = get_sub_matrices(A);
    for (auto matrix : sub) {
        print_matrix(matrix);
        std::cout << "-----" << std::endl;
    }
    std::cout << "-----block multiplication-----" << std::endl;
    print_matrix(block_mult(A,B));
    std::cout << "-----strassen multiplication-----" << std::endl;
    print_matrix(strassen_mult(A,B));

}

int main(){
    /*
    MatrixFactory factory = MatrixFactory();
    size_t matrix_size = 0;
    std::cin >> matrix_size;

    Matrix matrix1 = factory.get_random_matrix(matrix_size, 0, 9);
    Matrix matrix2 = factory.get_random_matrix(matrix_size, 0, 9);

    print_matrix(matrix1);
    std::cout << "------" << std::endl;
    print_matrix(matrix2);
    std::cout << "------block-----" << std::endl;
    print_matrix(block_mult(matrix1, matrix2));
    std::cout << "------strassen-----" << std::endl;
    print_matrix(strassen_mult(matrix1, matrix2));
    */
    tests();

    return 0;
}
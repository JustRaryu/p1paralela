#include <iostream>
#include "matrix_factory.h"
#include <vector>

Matrix case_base(Matrix A, Matrix B) {
    return Matrix{std::vector<int>{A[0][0]*B[0][0]}};
}

Matrix sub_matrix(Matrix parent, int row, int column) {
    auto it_row_begin = parent.cbegin();
    auto it_row_end = parent.cend();
    auto it_row_middle = it_row_begin+parent.size()/2;
 
    if (row == 0) {
        it_row_end = it_row_middle;     
    }
    else {
        it_row_begin = it_row_middle;
    }

    Matrix sub_matrix = Matrix(it_row_begin, it_row_end);

    for (size_t i = 0; i < sub_matrix.size();i++) {
        auto it_column_begin = sub_matrix[i].cbegin();
        auto it_column_end = sub_matrix[i].cend();
        auto it_column_middle = it_column_begin+sub_matrix[i].size()/2;

        if (column == 0) {
            it_column_end = it_column_middle;
        }
        else {
            it_column_begin = it_column_middle;
        }

        sub_matrix[i] = std::vector<int>(it_column_begin, it_column_end);
    }


    return sub_matrix;
}

Matrix matrix_multiplication(Matrix A, Matrix B){
    if (A.size() == 1 || B.size() == 1) {
        return case_base(A,B);
    }


    return Matrix();

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

    auto matrix1 = factory.get_random_matrix(matrix_size, 0, 9);
    auto matrix2 = factory.get_random_matrix(matrix_size, 0, 9);
    
    print_matrix(matrix1);
    std::cout << "------" << std::endl;
    print_matrix(sub_matrix(matrix1, 1, 1));

    return 0;
}
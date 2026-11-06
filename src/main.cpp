/**
 * @file main.cpp
 * @brief Демонстрация работы библиотеки «Калькулятор матриц».
 */
#include "matrix.h"

#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "=== Matrix Calculator (LR-3, variant 13) ===" << std::endl;

    // --- Часть 1. Базовые операции ---
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(2, 2);
    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;
    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;

    Matrix sum = matrix_add(A, B);
    Matrix product = matrix_multiply(A, B);

    std::cout << "\nMatrix A:" << std::endl;
    print_matrix(A);
    std::cout << "Matrix B:" << std::endl;
    print_matrix(B);
    std::cout << "Sum A + B:" << std::endl;
    print_matrix(sum);
    std::cout << "Product A x B:" << std::endl;
    print_matrix(product);

    // --- Часть 2. Вариант 13: выделение подматрицы ---
    double arr[] = { 1,  2,  3,  4,  5,
                     6,  7,  8,  9, 10,
                    11, 12, 13, 14, 15,
                    16, 17, 18, 19, 20};
    Matrix M = matrix_from_array(arr, 4, 5);
    std::cout << "\nMatrix M (4x5) from array:" << std::endl;
    print_matrix(M);

    Matrix S = matrix_submatrix(M, 1, 2, 2, 3);
    std::cout << "Submatrix M[1..2][2..4] = matrix_submatrix(M, 1, 2, 2, 3):" << std::endl;
    print_matrix(S);

    S.data[0][0] = 100; // копия независима от оригинала
    std::cout << "After S[0][0] = 100, M[1][2] = " << M.data[1][2] << std::endl;

    // --- Обработка ошибок ---
    std::cout << "\nError handling:" << std::endl;
    try {
        Matrix bad = matrix_submatrix(M, 3, 3, 2, 3); // выходит за границы
        free_matrix(bad);
    } catch (const std::out_of_range& e) {
        std::cout << "  out_of_range: " << e.what() << std::endl;
    }
    try {
        Matrix C = create_matrix(2, 3);
        try {
            Matrix bad = matrix_add(A, C); // разные размеры
            free_matrix(bad);
        } catch (const std::invalid_argument& e) {
            std::cout << "  invalid_argument: " << e.what() << std::endl;
        }
        free_matrix(C);
    } catch (const std::exception& e) {
        std::cout << "  unexpected: " << e.what() << std::endl;
    }

    // --- Освобождение памяти ---
    free_matrix(A);
    free_matrix(B);
    free_matrix(sum);
    free_matrix(product);
    free_matrix(M);
    free_matrix(S);

    std::cout << "\nDone, all memory freed." << std::endl;
    return 0;
}

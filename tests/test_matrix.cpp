/**
 * @file test_matrix.cpp
 * @brief Unit-тесты библиотеки «Калькулятор матриц» (Google Test).
 */
#include <gtest/gtest.h>

#include <stdexcept>

#include "matrix.h"

// ============================================================
// 1. НОРМАЛЬНЫЕ СЛУЧАИ
// ============================================================

// 1.1. Создание матрицы
TEST(MatrixTest, CreateMatrix_NormalCase) {
    Matrix A = create_matrix(3, 4);
    EXPECT_EQ(A.rows, 3);
    EXPECT_EQ(A.cols, 4);
    EXPECT_NE(A.data, nullptr);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_DOUBLE_EQ(A.data[i][j], 0.0);
    free_matrix(A);
}

// 1.2. Сложение матриц
TEST(MatrixTest, Addition_NormalCase) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(2, 2);
    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;
    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;
    Matrix C = matrix_add(A, B);
    EXPECT_DOUBLE_EQ(C.data[0][0], 6.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 8.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 10.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 12.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// 1.3. Умножение матриц
TEST(MatrixTest, Multiplication_NormalCase) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(2, 2);
    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;
    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;
    Matrix C = matrix_multiply(A, B);
    EXPECT_DOUBLE_EQ(C.data[0][0], 19.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 22.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 43.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 50.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// 1.4. Транспонирование
TEST(MatrixTest, Transpose_NormalCase) {
    Matrix A = create_matrix(2, 3);
    A.data[0][0] = 1; A.data[0][1] = 2; A.data[0][2] = 3;
    A.data[1][0] = 4; A.data[1][1] = 5; A.data[1][2] = 6;
    Matrix T = matrix_transpose(A);
    EXPECT_EQ(T.rows, 3);
    EXPECT_EQ(T.cols, 2);
    EXPECT_DOUBLE_EQ(T.data[0][0], 1.0);
    EXPECT_DOUBLE_EQ(T.data[0][1], 4.0);
    EXPECT_DOUBLE_EQ(T.data[1][0], 2.0);
    EXPECT_DOUBLE_EQ(T.data[1][1], 5.0);
    EXPECT_DOUBLE_EQ(T.data[2][0], 3.0);
    EXPECT_DOUBLE_EQ(T.data[2][1], 6.0);
    free_matrix(A);
    free_matrix(T);
}

// 1.5. Освобождение памяти — нормальный случай
TEST(MatrixTest, FreeMatrix_NormalCase) {
    Matrix A = create_matrix(3, 3);
    EXPECT_NO_THROW(free_matrix(A));
}

// 1.6. Создание матрицы из массива
TEST(MatrixTest, FromArray_NormalCase) {
    double arr[] = {1, 2, 3, 4, 5, 6};
    Matrix M = matrix_from_array(arr, 2, 3);
    EXPECT_EQ(M.rows, 2);
    EXPECT_EQ(M.cols, 3);
    EXPECT_DOUBLE_EQ(M.data[0][2], 3.0);
    EXPECT_DOUBLE_EQ(M.data[1][0], 4.0);
    EXPECT_DOUBLE_EQ(M.data[1][2], 6.0);
    free_matrix(M);
}

// ============================================================
// 2. ГРАНИЧНЫЕ СЛУЧАИ
// ============================================================

// 2.1. Минимальный размер матрицы — 1×1
TEST(MatrixTest, CreateMatrix_MinimumSize) {
    Matrix A = create_matrix(1, 1);
    EXPECT_EQ(A.rows, 1);
    EXPECT_EQ(A.cols, 1);
    EXPECT_DOUBLE_EQ(A.data[0][0], 0.0);
    free_matrix(A);
}

// 2.2. Вектор-строка × вектор-столбец = 1×1
TEST(MatrixTest, Multiplication_RowTimesColumn_1x1) {
    Matrix A = create_matrix(1, 3);
    Matrix B = create_matrix(3, 1);
    A.data[0][0] = 1; A.data[0][1] = 2; A.data[0][2] = 3;
    B.data[0][0] = 4; B.data[1][0] = 5; B.data[2][0] = 6;
    Matrix C = matrix_multiply(A, B);
    EXPECT_EQ(C.rows, 1);
    EXPECT_EQ(C.cols, 1);
    EXPECT_DOUBLE_EQ(C.data[0][0], 32.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// 2.3. Двойное транспонирование возвращает исходную матрицу
TEST(MatrixTest, TransposeTwice_ReturnsOriginal) {
    Matrix A = create_matrix(2, 3);
    A.data[0][0] = 1; A.data[0][1] = 2; A.data[0][2] = 3;
    A.data[1][0] = 4; A.data[1][1] = 5; A.data[1][2] = 6;
    Matrix T = matrix_transpose(A);
    Matrix TT = matrix_transpose(T);
    EXPECT_EQ(TT.rows, 2);
    EXPECT_EQ(TT.cols, 3);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(TT.data[i][j], A.data[i][j]);
    free_matrix(A);
    free_matrix(T);
    free_matrix(TT);
}

// ============================================================
// 3. НЕКОРРЕКТНЫЕ СЛУЧАИ
// ============================================================

// 3.1. Нулевое количество строк
TEST(MatrixTest, CreateMatrix_ZeroRows) {
    EXPECT_THROW(create_matrix(0, 3), std::invalid_argument);
}

// 3.2. Отрицательное количество столбцов
TEST(MatrixTest, CreateMatrix_NegativeCols) {
    EXPECT_THROW(create_matrix(3, -1), std::invalid_argument);
}

// 3.3. Сложение матриц разных размеров
TEST(MatrixTest, Addition_SizeMismatch) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(3, 3);
    EXPECT_THROW(matrix_add(A, B), std::invalid_argument);
    free_matrix(A);
    free_matrix(B);
}

// 3.4. Умножение матриц с несовместимыми размерами
TEST(MatrixTest, Multiplication_SizeMismatch) {
    Matrix A = create_matrix(2, 3);
    Matrix B = create_matrix(2, 2);
    EXPECT_THROW(matrix_multiply(A, B), std::invalid_argument);
    free_matrix(A);
    free_matrix(B);
}

// 3.5. Повторное освобождение пустой матрицы безопасно
TEST(MatrixTest, FreeMatrix_EmptyIsSafe) {
    Matrix empty{nullptr, 0, 0};
    EXPECT_NO_THROW(free_matrix(empty));
    EXPECT_NO_THROW(free_matrix(empty));
}

// ============================================================
// 4. ИНДИВИДУАЛЬНОЕ ЗАДАНИЕ (вариант 13): matrix_submatrix
// ============================================================

/// Тестовая фикстура: матрица 4×5 с элементами 1..20 по строкам.
class SubmatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        double arr[20];
        for (int k = 0; k < 20; ++k) arr[k] = k + 1;
        M = matrix_from_array(arr, 4, 5);
    }
    void TearDown() override { free_matrix(M); }
    Matrix M{nullptr, 0, 0};
};

// 4.1. Блок из середины матрицы
TEST_F(SubmatrixTest, MiddleBlock) {
    Matrix S = matrix_submatrix(M, 1, 2, 2, 3);
    ASSERT_EQ(S.rows, 2);
    ASSERT_EQ(S.cols, 3);
    EXPECT_DOUBLE_EQ(S.data[0][0], 8.0);
    EXPECT_DOUBLE_EQ(S.data[0][2], 10.0);
    EXPECT_DOUBLE_EQ(S.data[1][0], 13.0);
    EXPECT_DOUBLE_EQ(S.data[1][2], 15.0);
    free_matrix(S);
}

// 4.2. Подматрица, совпадающая со всей матрицей
TEST_F(SubmatrixTest, WholeMatrix) {
    Matrix S = matrix_submatrix(M, 0, 0, 4, 5);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
            EXPECT_DOUBLE_EQ(S.data[i][j], M.data[i][j]);
    free_matrix(S);
}

// 4.3. Один элемент в правом нижнем углу (граничный случай)
TEST_F(SubmatrixTest, SingleElementCorner) {
    Matrix S = matrix_submatrix(M, 3, 4, 1, 1);
    EXPECT_EQ(S.rows, 1);
    EXPECT_EQ(S.cols, 1);
    EXPECT_DOUBLE_EQ(S.data[0][0], 20.0);
    free_matrix(S);
}

// 4.4. Подматрица — независимая копия
TEST_F(SubmatrixTest, IsIndependentCopy) {
    Matrix S = matrix_submatrix(M, 0, 0, 2, 2);
    S.data[0][0] = 100.0;
    EXPECT_DOUBLE_EQ(M.data[0][0], 1.0);
    EXPECT_NE(S.data[0], M.data[0]);
    free_matrix(S);
}

// 4.5. Выход блока за границы
TEST_F(SubmatrixTest, OutOfBounds) {
    EXPECT_THROW(matrix_submatrix(M, 3, 0, 2, 1), std::out_of_range);
    EXPECT_THROW(matrix_submatrix(M, 0, 4, 1, 2), std::out_of_range);
    EXPECT_THROW(matrix_submatrix(M, -1, 0, 1, 1), std::out_of_range);
    EXPECT_THROW(matrix_submatrix(M, 0, -1, 1, 1), std::out_of_range);
    EXPECT_THROW(matrix_submatrix(M, 4, 0, 1, 1), std::out_of_range);
}

// 4.6. Неположительные размеры подматрицы
TEST_F(SubmatrixTest, NonPositiveSize) {
    EXPECT_THROW(matrix_submatrix(M, 0, 0, 0, 2), std::invalid_argument);
    EXPECT_THROW(matrix_submatrix(M, 0, 0, 2, -3), std::invalid_argument);
}

// 4.7. Пустая исходная матрица
TEST(MatrixTest, Submatrix_EmptySource) {
    Matrix empty{nullptr, 0, 0};
    EXPECT_THROW(matrix_submatrix(empty, 0, 0, 1, 1), std::invalid_argument);
}

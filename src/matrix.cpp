/**
 * @file matrix.cpp
 * @brief Реализация функций библиотеки «Калькулятор матриц».
 */
#include "matrix.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <new>
#include <stdexcept>

namespace {

/// @brief Проверка, что матрица не пустая (есть данные и положительные размеры).
bool is_valid(const Matrix& m) {
    return m.data != nullptr && m.rows > 0 && m.cols > 0;
}

} // namespace

Matrix create_matrix(int rows, int cols) {
    // 1. Проверка корректности размеров
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive integers");
    }

    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;

    // 2. Выделение памяти для массива указателей на строки
    mat.data = new double*[rows];

    // 3. Выделение памяти для каждой строки и инициализация нулями
    for (int i = 0; i < rows; i++) {
        try {
            mat.data[i] = new double[cols];
        } catch (const std::bad_alloc&) {
            // Освобождение уже выделенной памяти при ошибке
            for (int j = 0; j < i; j++) {
                delete[] mat.data[j];
            }
            delete[] mat.data;
            throw;
        }
        for (int j = 0; j < cols; j++) {
            mat.data[i][j] = 0.0;
        }
    }
    return mat;
}

void free_matrix(Matrix m) {
    // 1. Защита от освобождения пустой матрицы
    if (m.data == nullptr) {
        return;
    }
    // 2. Освобождение памяти каждой строки
    for (int i = 0; i < m.rows; i++) {
        delete[] m.data[i];
        m.data[i] = nullptr;
    }
    // 3. Освобождение массива указателей
    delete[] m.data;
    // 4. Обнуление локальной копии структуры
    m.data = nullptr;
    m.rows = 0;
    m.cols = 0;
}

Matrix matrix_add(Matrix a, Matrix b) {
    if (!is_valid(a) || !is_valid(b)) {
        throw std::invalid_argument("matrix_add: empty matrix");
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("matrix_add: matrices must have the same size");
    }
    Matrix c = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            c.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return c;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    if (!is_valid(a) || !is_valid(b)) {
        throw std::invalid_argument("matrix_multiply: empty matrix");
    }
    if (a.cols != b.rows) {
        throw std::invalid_argument("matrix_multiply: cols(a) must equal rows(b)");
    }
    Matrix c = create_matrix(a.rows, b.cols); // уже заполнена нулями
    // Порядок i-k-j: строки b и c читаются/пишутся последовательно
    for (int i = 0; i < a.rows; i++) {
        for (int k = 0; k < a.cols; k++) {
            const double aik = a.data[i][k];
            for (int j = 0; j < b.cols; j++) {
                c.data[i][j] += aik * b.data[k][j];
            }
        }
    }
    return c;
}

Matrix matrix_transpose(Matrix m) {
    if (!is_valid(m)) {
        throw std::invalid_argument("matrix_transpose: empty matrix");
    }
    Matrix t = create_matrix(m.cols, m.rows);
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            t.data[j][i] = m.data[i][j];
        }
    }
    return t;
}

Matrix matrix_submatrix(Matrix m, int start_row, int start_col, int rows, int cols) {
    // 1. Исходная матрица должна существовать
    if (!is_valid(m)) {
        throw std::invalid_argument("matrix_submatrix: source matrix is empty");
    }
    // 2. Размеры подматрицы должны быть положительными
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("matrix_submatrix: rows and cols must be positive");
    }
    // 3. Блок должен целиком лежать внутри исходной матрицы
    //    (сравнение через вычитание исключает переполнение int при сложении)
    if (start_row < 0 || start_col < 0 ||
        rows > m.rows - start_row || cols > m.cols - start_col) {
        throw std::out_of_range("matrix_submatrix: block is out of matrix bounds");
    }
    // 4. Копирование блока в новую независимую матрицу
    Matrix sub = create_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sub.data[i][j] = m.data[start_row + i][start_col + j];
        }
    }
    return sub;
}

void print_matrix(Matrix m) {
    // 1. Проверка валидности матрицы
    if (!is_valid(m)) {
        std::cout << "[Empty matrix]" << std::endl;
        return;
    }

    // 2. Настройка формата вывода (с восстановлением после печати)
    const std::ios_base::fmtflags old_flags = std::cout.flags();
    const std::streamsize old_precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(2);

    // 3. Сокращённый вывод для больших матриц
    const int MAX_ROWS_TO_SHOW = 10;
    const int MAX_COLS_TO_SHOW = 8;
    const bool cut_rows = m.rows > MAX_ROWS_TO_SHOW;
    const bool cut_cols = m.cols > MAX_COLS_TO_SHOW;
    const int half_r = MAX_ROWS_TO_SHOW / 2;
    const int half_c = MAX_COLS_TO_SHOW / 2;

    auto print_row = [&](int i) {
        std::cout << "[";
        if (!cut_cols) {
            for (int j = 0; j < m.cols; j++) {
                std::cout << std::setw(8) << m.data[i][j];
            }
        } else {
            for (int j = 0; j < half_c; j++) std::cout << std::setw(8) << m.data[i][j];
            std::cout << "     ...";
            for (int j = m.cols - half_c; j < m.cols; j++) std::cout << std::setw(8) << m.data[i][j];
        }
        std::cout << " ]" << std::endl;
    };

    // 4. Вывод строк (первые и последние для больших матриц)
    if (!cut_rows) {
        for (int i = 0; i < m.rows; i++) print_row(i);
    } else {
        for (int i = 0; i < half_r; i++) print_row(i);
        std::cout << "[     ...  ]" << std::endl;
        for (int i = m.rows - half_r; i < m.rows; i++) print_row(i);
    }

    // 5. Информация о размере для больших матриц
    if (cut_rows || cut_cols) {
        std::cout << "Matrix " << m.rows << "x" << m.cols << " (showing "
                  << std::min(m.rows, MAX_ROWS_TO_SHOW) << "x"
                  << std::min(m.cols, MAX_COLS_TO_SHOW) << ")" << std::endl;
    }

    std::cout.flags(old_flags);
    std::cout.precision(old_precision);
}

Matrix matrix_from_array(double* data, int rows, int cols) {
    if (data == nullptr) {
        throw std::invalid_argument("matrix_from_array: data is nullptr");
    }
    Matrix m = create_matrix(rows, cols); // проверит rows/cols
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = data[i * cols + j];
        }
    }
    return m;
}

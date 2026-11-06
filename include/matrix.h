/**
 * @file matrix.h
 * @brief Библиотека «Калькулятор матриц»: структура Matrix и операции над ней.
 * @author Кожаев Т. В., ИУ5-13/4
 * @version 1.0
 *
 * Лабораторная работа № 3 «Полный цикл разработки калькулятора матриц на C++».
 * Индивидуальное задание — вариант 13: выделение подматрицы (matrix_submatrix).
 */
#ifndef MATRIX_H
#define MATRIX_H

/**
 * @brief Структура матрицы.
 *
 * Хранит двумерный динамический массив чисел типа double и его размеры.
 * @warning Память, выделенная create_matrix() и функциями, возвращающими Matrix,
 *          освобождается только через free_matrix().
 */
typedef struct {
    double** data; ///< Указатель на двумерный массив (массив указателей на строки).
    int rows;      ///< Количество строк.
    int cols;      ///< Количество столбцов.
} Matrix;

/**
 * @brief Создание матрицы заданного размера.
 *
 * @param rows Количество строк (> 0).
 * @param cols Количество столбцов (> 0).
 * @return Matrix Матрица rows×cols, все элементы которой равны 0.0.
 *
 * @exception std::invalid_argument Если rows <= 0 или cols <= 0.
 * @exception std::bad_alloc Если не удалось выделить память.
 *
 * @note Время выполнения: O(rows * cols).
 * @see free_matrix()
 *
 * @code
 * Matrix A = create_matrix(3, 3);
 * free_matrix(A);
 * @endcode
 */
Matrix create_matrix(int rows, int cols);

/**
 * @brief Освобождение памяти, занятой матрицей.
 *
 * Освобождает каждую строку, затем массив указателей на строки.
 * Безопасна для «пустой» матрицы (data == nullptr).
 *
 * @param m Матрица, память которой нужно освободить.
 *
 * @note Структура передаётся по значению, поэтому указатель data у вызывающей
 *       стороны не обнуляется: защита от double free / use-after-free —
 *       задача вызывающего кода.
 * @see create_matrix()
 */
void free_matrix(Matrix m);

/**
 * @brief Сложение двух матриц: C = A + B, c_ij = a_ij + b_ij.
 *
 * @param a Первая матрица M×N.
 * @param b Вторая матрица M×N.
 * @return Matrix Новая матрица M×N — сумма a и b.
 *
 * @exception std::invalid_argument Если одна из матриц пустая или размеры не совпадают.
 *
 * @note Время выполнения: O(M*N).
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_add(Matrix a, Matrix b);

/**
 * @brief Умножение двух матриц: C = A × B, c_ij = Σ_k a_ik · b_kj.
 *
 * @param a Матрица M×N.
 * @param b Матрица N×K.
 * @return Matrix Результат M×K.
 *
 * @exception std::invalid_argument Если одна из матриц пустая или cols(a) != rows(b).
 *
 * @note Время выполнения: O(M*N*K). Используется порядок циклов i-k-j,
 *       при котором строки b и c обходятся последовательно (дружелюбно к кэшу).
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_multiply(Matrix a, Matrix b);

/**
 * @brief Транспонирование матрицы: T = Aᵀ, t_ji = a_ij.
 *
 * Вспомогательная функция, используемая в unit-тестах из методических указаний.
 *
 * @param m Исходная матрица M×N.
 * @return Matrix Транспонированная матрица N×M.
 *
 * @exception std::invalid_argument Если матрица пустая.
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_transpose(Matrix m);

/**
 * @brief Выделение подматрицы (индивидуальное задание, вариант 13).
 *
 * Возвращает новую матрицу rows×cols, скопированную из блока исходной матрицы,
 * левый верхний угол которого находится в позиции (start_row, start_col).
 *
 * @param m         Исходная матрица.
 * @param start_row Индекс первой строки блока (с 0).
 * @param start_col Индекс первого столбца блока (с 0).
 * @param rows      Количество строк подматрицы (> 0).
 * @param cols      Количество столбцов подматрицы (> 0).
 * @return Matrix Независимая копия блока размером rows×cols.
 *
 * @exception std::invalid_argument Если исходная матрица пустая или rows/cols <= 0.
 * @exception std::out_of_range Если блок выходит за границы исходной матрицы
 *            (start_row < 0, start_col < 0, start_row + rows > m.rows
 *            или start_col + cols > m.cols).
 *
 * @note Время выполнения: O(rows * cols). Изменение подматрицы не меняет оригинал.
 * @warning Результат нужно освобождать через free_matrix().
 *
 * @code
 * // A — матрица 4×5, берём блок 2×3, начиная со строки 1 и столбца 2
 * Matrix S = matrix_submatrix(A, 1, 2, 2, 3);
 * free_matrix(S);
 * @endcode
 */
Matrix matrix_submatrix(Matrix m, int start_row, int start_col, int rows, int cols);

/**
 * @brief Вывод матрицы в консоль с выравниванием по столбцам.
 *
 * Числа выводятся с двумя знаками после запятой. Матрицы больше 10×8
 * выводятся в сокращённом виде с многоточиями.
 *
 * @param m Матрица для вывода. Для пустой матрицы печатается «[Empty matrix]».
 */
void print_matrix(Matrix m);

/**
 * @brief Создание матрицы из одномерного массива (заполнение по строкам).
 *
 * data[0..cols-1] — первая строка, data[cols..2*cols-1] — вторая и т. д.
 *
 * @param data Указатель на массив из rows*cols элементов.
 * @param rows Количество строк (> 0).
 * @param cols Количество столбцов (> 0).
 * @return Matrix Новая матрица rows×cols с данными из массива.
 *
 * @exception std::invalid_argument Если data == nullptr или rows/cols <= 0.
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_from_array(double* data, int rows, int cols);

#endif // MATRIX_H

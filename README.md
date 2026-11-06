# Matrix Calculator

Лабораторная работа № 3 «Полный цикл разработки калькулятора матриц на C++».
Кожаев Т. В., ИУ5-13/4. Индивидуальное задание — вариант 13: `matrix_submatrix`.

## Сборка и тесты
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
cd build && ctest --verbose
```

## Документация
```bash
doxygen Doxyfile   # результат: docs/html/index.html
```

## Docker
```bash
docker build -t matrix-calculator .
docker run -it --rm matrix-calculator
```

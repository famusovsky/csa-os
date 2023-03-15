#include <stdio.h>
#include <stdlib.h>

// Вариант 13
// Разработать программу, вычисляющую с помощью степенного ряда с точностью
// не хуже 0,1% значение функции e^x для заданного параметра x.

// Функция производит вычисление степенного ряда с максимальной точностью,
// т.е. до тех пор, пока добавление очередного члена ряда изменяет его сумму.

double ex(double x) {
    long double sum = 0;
    long double result_sum = -1;
    long double step = 1;
    long double n = 0;
    while (sum != result_sum) {
        result_sum = sum;
        sum += step;
        step = step * x / ++n;
    }
    return result_sum;
}

// Программа запускается из терминала (командной строки); 
// x передаётся в качестве аргумента командной строки.

int main(int argc, char **argv) {
    int x = atoi(argv[1]);
    if (x >= 0) {
        printf("%lf \n", ex(x));
    } else {
        printf("%lf \n", 1 / ex(-x));
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

// Вариант 11
// Сформировать массив B из элементов A, расположенных в обратном
// порядке, исключая первый (с начала, согласно комментарию Легалова А.И.) положительный элемент.

int makeArrayB(const int *a, int *b, int n) {
  int i = 0, j = 0, first_pos = -1;
  for (; i < n; ++i) {
    if (a[i] > 0) {
      first_pos = i;
      break;
    }
  }
  i = n - 1;
  for (; i >= 0; --i) {
    if (i != first_pos) {
      b[j] = a[i];
      ++j;
    }
  }
  if (first_pos == -1) {
    return 0;
  }
  return 1;
}

void outputArray(const int *arr, int n) {
  for (int i = 0; i < n; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

// Программа запускается из терминала (командной строки); 
// В качестве аргументов командной строки передаётся данный массив в виде чисел, разделенных пробелами.

int main(int argc, char **argv) {
  int *a = (int *) malloc((argc - 1) * sizeof(int));
  int *b = (int *) malloc((argc - 1) * sizeof(int));
  for (int i = 1; i < argc; ++i) {
    a[i - 1] = atoi(argv[i]);
  }
  int size_dif = makeArrayB(a, b, argc - 1);
  outputArray(b, argc - 1 - size_dif);
}
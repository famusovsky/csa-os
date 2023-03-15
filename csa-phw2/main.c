#include <stdio.h>
#include <stdlib.h>

// Вариант 4
// Разработать программу, находящую в заданной ASCII-строке последнюю при
// перемещении слева направо последовательность N символов, каждый элемент
// которой определяется по условию «больше предшествующего» (N вводится как
// отдельный параметр, N > 1).

char *findLastSeq(char *str, int n) {
  char *last_seq = (char *)malloc(n * sizeof(char));
  int i = 0;
  int is_seq_correct = 0;
  while (str[i + n - 1] != EOF) {
    for (int j = 1 ; j < n ; j++) {
      if (str[i + j] > str[i + j - 1]) {
        is_seq_correct = 1;
      } else {
        is_seq_correct = 0;
        break;
      }
    }
    if (is_seq_correct) {
      for (int j = 0 ; j < n ; j++) {
        last_seq[j] = str[i + j];
      }
    }
    ++i;
  }
  return last_seq;
}

// Строка вводится с клавиатуры до ввода символа EOF (Ctrl+D)

char *inputLine() {
  char *str = (char *)malloc(1 * sizeof(char));
  char c = getchar();
  int i = 0;
  while (c != EOF) {
    str[i] = c;
    ++i;
    str = (char *)realloc(str, (i + 1) * sizeof(char));
    c = getchar();
  }
  str[i] = EOF;
  return str;
}

// Вначале вводится число N, затем через Enter вводится строка

int main(int argc, char **argv) {
  int n = 0;
  scanf("%d", &n);
  char *str = inputLine();
  char *last_seq = findLastSeq(str, n);
  printf("\n");
  printf("%s", last_seq);
  printf("\n");
}
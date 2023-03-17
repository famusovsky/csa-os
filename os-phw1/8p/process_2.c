// Степанов Алексей
// Вариант 34
// Разработать программу, которая на основе анализа двух ASCII строк формирует
// на выходе строку, содержащую символы, присутствующие в одной или другой
// (объединение символов). Каждый символ в соответствующей выходной строке
// должен встречаться только один раз.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 5000

// Процесс 2 -- -- обработка данных из канала fifo1 и запись данных в канал fifo2
int main(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE];
  // Открываем именованные каналы fifo1 для чтения и fifo2 для записи
  int fd1 = open("fifo1", O_RDONLY);
  int fd2 = open("fifo2", O_WRONLY);

  // Чтение данных из канала fifo1 в буфер
  ssize_t read_size = read(fd1, buffer, BUFFER_SIZE);
  if (read_size < 0) {
    perror("read2");
    exit(EXIT_FAILURE);
  }

  // Обработка данных
  int current_lines_count = 0;
  int index = 0;
  char result[read_size]; // Буфер для хранения результата
  for (int i = 0; i < read_size; i++) {
    // При окончании второй строки прекращаем обработку данных
    if (buffer[i] == '\n' && current_lines_count == 0) {
      current_lines_count++;
      continue;
    } else if (buffer[i] == '\n' && current_lines_count > 0) {
      break;
    }

    // Поиск текущего символа среди уже пройденных символов
    int is_char_found = 0;
    for (int j = 0; j < i; j++) {
      if (buffer[i] == buffer[j]) {
        is_char_found = 1;
        break;
      }
    }
    // Если символ встречается впервые, добавляем его в выходную строку
    if (is_char_found == 0) {
      result[index] = buffer[i];
      index++;
    }
  }

  write(fd2, result, index); // Запись данных из буфера result в канал fifo2
  close(fd1); // Закрытие файловых дескрипторов канала fifo1
  close(fd2); // Закрытие файловых дескрипторов канала fifo2
  return EXIT_SUCCESS; // Завершение дочернего процесса
}
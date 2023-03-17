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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 5000

int main(int argc, char *argv[]) {
  // Проверка наличия двух аргументов командной строки: входного файла и выходного файла
  if (argc != 3) {
    printf("Usage: %s input_file output_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Создание трёх процессов
  pid_t pid1, pid2;

  unlink("fifo1"); // На случай, если каналы уже существуют
  unlink("fifo2"); // На случай, если каналы уже существуют
  // Проверка ошибок при создании каналов
  if (mkfifo("fifo1", 0666) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }
  if (mkfifo("fifo2", 0666) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  if ((pid1 = fork()) < 0) {
    perror("fork1");
    exit(EXIT_FAILURE);
  } else if (pid1 == 0) {
    // Дочерний процесс 1 -- чтение данных из входного файла
    // Объявление буфера
    char buffer[BUFFER_SIZE];
    // Открытие входного файла и канала fifo1
    int input_fd = open(argv[1], O_RDONLY);
    int fd1 = open("fifo1", O_WRONLY);

    // Чтение данных из входного файла в буфер
    ssize_t read_size = read(input_fd, buffer, BUFFER_SIZE);
    if (read_size < 0) {
      perror("read1");
      exit(EXIT_FAILURE);
    }

    // Запись данных из буфера buffer в канал fifo1
    write(fd1, buffer, read_size);
    close(input_fd); // Закрытие файлового дескриптора входного файла
    close(fd1); // Закрытие файлового дескриптора канала fifo1

    // Открываем именованный канал fifo2 для чтения
    int fd2 = open("fifo2", O_RDONLY);
    // Открытие выходного файла
    int output_fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    read_size = read(fd2, buffer, BUFFER_SIZE); // Чтение данных из канала fifo2 в буфер buffer1
    if (read_size < 0) { // Проверка ошибок при чтении данных из канала
      perror("read3");
      exit(EXIT_FAILURE);
    }
    write(output_fd, buffer, read_size); // Запись данных из буфера buffer1 в выходной файл
    close(output_fd); // Закрытие файлового дескриптора выходного файла
    close(fd2); // Закрытие файловых дескрипторов канала fifo2

    exit(EXIT_SUCCESS); // Завершение дочернего процесса
  }

  // Создание второго процесса
  if ((pid2 = fork()) < 0) { // Проверка ошибок при создании процесса
    perror("fork2");
    exit(EXIT_FAILURE);
  } else if (pid2 == 0) {
    // Дочерний процесс 2 -- обработка данных
    // Объявление буфера
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
    exit(EXIT_SUCCESS); // Завершение дочернего процесса
  }

  waitpid(pid1, NULL, 0); //
  waitpid(pid2, NULL, 0); // Ожидание завершения всех дочерних процессов

  unlink("fifo1"); // Удаление канала fifo1
  unlink("fifo2"); // Удаление канала fifo2

  return EXIT_SUCCESS;
}

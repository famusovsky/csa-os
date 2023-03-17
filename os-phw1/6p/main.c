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

#define BUFFER_SIZE 5000

int main(int argc, char *argv[]) {
  // Проверка наличия двух аргументов командной строки: входного файла и
  // выходного файла
  if (argc != 3) {
    printf("Usage: %s input_file output_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Создание двух каналов для обмена данными между процессами
  int pipe1[2], pipe2[2];
  // Создание трёх процессов
  pid_t pid1, pid2;

  // Проверка ошибок при создании каналов
  if (pipe(pipe1) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  if (pipe(pipe2) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((pid1 = fork()) < 0) { // Проверка ошибок при создании процесса
    perror("fork1");
    exit(EXIT_FAILURE);
  } else if (pid1 == 0) {
    // Дочерний процесс 1 -- чтение данных из входного файла
    close(pipe1[0]); // Закрытие неиспользуемого конца канала pipe1
    // Объявление буфера
    char buffer[BUFFER_SIZE];

    // Открытие входного файла и чтение данных в буфер buffer
    int input_fd = open(argv[1], O_RDONLY);
    ssize_t read_size = read(input_fd, buffer, BUFFER_SIZE);
    if (read_size < 0) {
      perror("read1");
      exit(EXIT_FAILURE);
    }
    // Запись данных из буфера buffer1 в канал pipe1
    write(pipe1[1], buffer, read_size);
    close(input_fd); // Закрытие файлового дескриптора входного файла
    close(pipe1[1]); // Закрытие конца канала, используемого для записи
    close(pipe2[1]); // Закрытие конца канала pipe2, используемого для записи
    
    // Открытие выходного файла
    int output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    read_size = read(pipe2[0], buffer, BUFFER_SIZE); // Чтение данных из канала pipe2 в буфер buffer1
    if (read_size < 0) { // Проверка ошибок при чтении данных из канала
      perror("read3");
      exit(EXIT_FAILURE);
    }
    write(output_fd, buffer, read_size); // Запись данных из буфера buffer1 в выходной файл
    close(pipe2[0]);
    close(output_fd);

    exit(EXIT_SUCCESS); // Завершение дочернего процесса
  }

  // Создание второго процесса
  if ((pid2 = fork()) < 0) { // Проверка ошибок при создании процесса
    perror("fork2");
    exit(EXIT_FAILURE);
  } else if (pid2 == 0) {
    // Дочерний процесс 2 -- обработка данных
    close(pipe1[1]); // Закрытие конца канала pipe1, используемого для записи
    close(pipe2[0]); // Закрытие неиспользуемого конца канала pipe2
    // Объявление буфера
    char buffer[BUFFER_SIZE];
    // Чтение данных из канала pipe1 в буфер buffer2
    ssize_t read_size = read(pipe1[0], buffer, BUFFER_SIZE);
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

    write(pipe2[1], result, index); // Запись данных из буфера result в канал pipe2
    close(pipe1[0]); // Закрытие конца канала, используемого для чтения
    close(pipe2[1]); // Закрытие конца канала, используемого для записи
    exit(EXIT_SUCCESS);
  }

  waitpid(pid1, NULL, 0); //
  waitpid(pid2, NULL, 0); // Ожидание завершения всех процессов

  close(pipe1[0]); //
  close(pipe1[1]); // Закрытие концов каналов, используемых для чтения и записи
  close(pipe2[0]); //
  close(pipe2[1]); //

  return EXIT_SUCCESS;
}

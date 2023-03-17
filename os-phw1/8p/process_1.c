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

// Процесс 1 -- запись данных из входного файла в канал fifo1 и запись данных из
// канала fifo2 в выходной файл
int main(int argc, char *argv[]) {
  // Проверка наличия двух аргументов командной строки: входного файла и
  // выходного файла
  if (argc != 3) {
    printf("Usage: %s input_file output_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

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
  int output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  read_size = read(fd2, buffer, BUFFER_SIZE); // Чтение данных из канала fifo2 в буфер buffer1
  if (read_size < 0) { // Проверка ошибок при чтении данных из канала
    perror("read3");
    exit(EXIT_FAILURE);
  }
  write(output_fd, buffer, read_size); // Запись данных из буфера buffer1 в выходной файл
  close(output_fd); // Закрытие файлового дескриптора выходного файла
  close(fd2); // Закрытие файловых дескрипторов канала fifo2

  unlink("fifo1"); // Удаление каналов
  unlink("fifo2"); // Удаление каналов
  return EXIT_SUCCESS; // Завершение дочернего процесса
}
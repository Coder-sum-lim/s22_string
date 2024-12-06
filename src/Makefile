TARGET = test_program
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lcheck -lm -pthread

all: $(TARGET)

$(TARGET): s21_strerror s21_strlen test # справа добавляем название зависимости # снизу добавляем .о файл
	$(CC) $(CFLAGS) -o $(TARGET) s21_strerror.o test.o s21_strlen.o $(LIBS)

# снизу добавляем правила для компиляции отдельных функций в файлах

s21_strerror:
	$(CC) $(CFLAGS) -c s21_strerror.c -o s21_strerror.o

test:
	$(CC) $(CFLAGS) -c test.c -o test.o

s21_strlen:
	$(CC) $(CFLAGS) -c s21_strlen.c -o s21_strlen.o

clean:
	rm -rf *.o $(TARGET)
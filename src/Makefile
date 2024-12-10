TARGET = test_program
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lcheck -lm -pthread

all: $(TARGET)

$(TARGET): s21_sprintf s21_strerror s21_strpbrk s21_strlen s21_memchr s21_memcmp s21_memcpy testing # справа добавляем название зависимости # снизу добавляем .о файл
	$(CC) $(CFLAGS) -o $(TARGET) s21_strerror.o test.o s21_strlen.o s21_strpbrk.o s21_memchr.o s21_memcmp.o s21_memcpy.o s21_sprintf.o $(LIBS)

# снизу добавляем правила для компиляции отдельных функций в файлах

s21_strerror:
	$(CC) $(CFLAGS) -c s21_strerror.c -o s21_strerror.o

testing:
	$(CC) $(CFLAGS) -c test.c -o test.o

s21_strlen:
	$(CC) $(CFLAGS) -c s21_strlen.c -o s21_strlen.o

s21_memchr:
	$(CC) $(CFLAGS) -c s21_memchr.c -o s21_memchr.o

s21_memcmp:
	$(CC) $(CFLAGS) -c s21_memcmp.c -o s21_memcmp.o

s21_memcpy:
	$(CC) $(CFLAGS) -c s21_memcpy.c -o s21_memcpy.o

s21_sprintf:
	$(CC) $(CFLAGS) -c s21_sprintf.c -o s21_sprintf.o

s21_strpbrk:
	$(CC) $(CFLAGS) -c s21_strpbrk.c -o s21_strpbrk.o

clang-do:
	cp ../../materials/linters/.clang-format .
	clang-format -i *.c *.h
	rm -rf .clang-format

clang-chek:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -rf .clang-format

test:
	./test_program

clean:
	rm -rf *.o $(TARGET)
# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g

# Ficheiros fonte e cabeçalhos
MAIN = main.c projeto.c
TEST = testes.c projeto.c
HDRS = projeto.h

# Def 'all'
all: main teste

# Comandos main
main:
	$(CC) $(CFLAGS) -o main $(MAIN)

# Comandos testes
teste:
	$(CC) $(CFLAGS) -g -o teste $(TEST) -lcunit

# Compila e corre o programa (main)
run: main
	@echo "A correr o programa..."
	@./main

# Compila e corre os testes
test: teste
	@echo "A correr os testes..."
	@./teste

# Código com gcov
gcov:
	$(CC) -Wall -Wextra -pedantic -O0 -fprofile-arcs -ftest-coverage -g -o teste_cov $(TEST) -lcunit
	./teste_cov
	gcov -b -c projeto.c

# Apaga executáveis e ficheiros temporários
clean:
	rm -f main teste teste_cov *.o *.gcda *.gcno *.gcov

# Comando Gerais
rebuild:
	make clean
	make all

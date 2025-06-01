# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g

# Ficheiros fonte e cabeçalhos
MAIN = main.c projeto.c
TEST = testes.c projeto.c
HDRS = projeto.h

# Def 'all'
all: test gcov compProj compMain run 

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
	$(CC) -Wall -Wextra -pedantic -O1 -fprofile-arcs -ftest-coverage -g -o teste_cov $(TEST) -lcunit
	./teste_cov
	gcov -b -c teste_cov-projeto.c

# Apaga executáveis e ficheiros temporários
clean:
	rm -f main teste teste_cov *.o *.gcda *.gcno *.gcov

# Comando Gerais
rebuild:
	make clean
	make all

# pmccabe projeto.c
compProj: 
	@echo "CompC   STMTS   LINHAS  INICIO  FIM     FUNC " 
	@pmccabe projeto.c 

# pmccabe main.c
compMain: 
	@echo "CompC   STMTS   LINHAS  INICIO  FIM     FUNC " 
	@pmccabe main.c 

# mostra comandos disponíveis
help:
	@echo "Comandos disponíveis:"
	@echo "  make all      		- Compila todos os ficheiros"
	@echo "  make run    			- Executa o jogo"
	@echo "  make test   			- Compila e executa os testes"
	@echo "  make gcov   			- Gera relatório de cobertura"
	@echo "  make clean  			- Remove ficheiros temporários"
	@echo "  make rebuild			- Limpa e recompila todos os ficheiros"
	@echo "  make comp(Proj/Main)		- pmccabe de projeto.c e main.c respetivamente"

#ifndef PROJETO_H
#define PROJETO_H

// Declarar as funções
void imprimirTabuleiro(char tabuleiro[1000][1000], int linhas, int colunas);
void pintarDeBranco(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void riscar(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void imprimir_comandos();
int formatoParaCoordenadas(char *input, int *x, int *y);
void gravarJogo(char *nome, Tabuleiro *t);
void lerJogo(char *nome, Tabuleiro *t);
void verificar_riscadas(char tabuleiro[1000][1000], int linhas, int colunas);
void verificar_brancas(char tabuleiro[1000][1000], int linhas, int colunas) ;

// Variaveis globais
#define STACK_SIZE 100
typedef struct {
    char tabuleiro[1000][1000];
    int linhas;
    int colunas;
} Tabuleiro;

extern Tabuleiro stack[STACK_SIZE];
extern int topoStack;

void stacks(Tabuleiro estado);
Tabuleiro desempilhar();


#endif

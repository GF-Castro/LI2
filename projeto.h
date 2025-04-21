#ifndef PROJETO_H
#define PROJETO_H

// Declaração de funções
void lerEstadoJogo(char *nomearquivo, char tabuleiro[1000][1000], int *linhas, int *colunas);
void imprimirTabuleiro(char tabuleiro[1000][1000], int linhas, int colunas);
void pintarDeBranco(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void riscar(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void imprimir_comandos();
int formatoParaCoordenadas(char *input, int *x, int *y);

// Declaração de variáveis globais
#define tamanhoStack 100

typedef struct {
    char tabuleiro[1000][1000];
    int linhas;
    int colunas;
} Tabuleiro;

Tabuleiro stack[tamanhoStack];
int topoStack = -1;

void stacks(Tabuleiro estado);
Tabuleiro desempilhar();


#endif
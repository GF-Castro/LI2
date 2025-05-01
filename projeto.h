#ifndef PROJETO_H
#define PROJETO_H

// Definição de constantes e structs
#define tamanhoStack 100

typedef struct {
    char tabuleiro[26][1000];
    int linhas;
    int colunas;
} Tabuleiro;

// Declaração de variáveis globais, usei extern para evitar duplicação, basicamente só diz que a variável existe em noutro arquivo (projeto.c)
extern Tabuleiro stack[tamanhoStack];
extern int topoStack;

// Declaração de funções
void imprimir_comandos();
void imprimirTabuleiro(char tabuleiro[26][1000], int linhas, int colunas);
void pintarDeBranco(char tabuleiro[26][1000], int linhas, int colunas, int x, int y);
void riscar(char tabuleiro[26][1000], int linhas, int colunas, int x, int y);
int pintar_vizinhos_de_branco(Tabuleiro *t, int i, int j);
int aplicar_inferencia(Tabuleiro *t);
void verificar_estado(char tabuleiro[26][1000], int linhas, int colunas);
void lerJogo(char *nome, Tabuleiro *t);
void gravarJogo(char *nome, Tabuleiro *t);
void guardar_estado(Tabuleiro *t);
void desfazer(Tabuleiro *t);
int formatoParaCoordenadas(char *input, int *x, int *y);
Tabuleiro desempilhar();
void stacks(Tabuleiro estado);
void verificar_riscadas(char tabuleiro[26][1000], int linhas, int colunas);
void verificar_brancas(char tabuleiro[26][1000], int linhas, int colunas);
void gravarStack(char *nome);
void lerStack(char *nome);

#endif
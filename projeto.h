#ifndef PROJETO_H
#define PROJETO_H
#include <stdbool.h>
#include <stdio.h>
// Definição de constantes e structs
#define tamanhoStack 100

typedef struct {
    int linhas;
    int colunas;
    char **tabuleiro;
} Tabuleiro;

typedef struct {
    char ação; // 'r' (riscar), 'b' (pintar)
    int x, y;    // coluna, linha
    char ant_val;
    char novo_val;
} Move;

// Declaração de variáveis globais, extern para evitar duplicação
extern Move movestack[tamanhoStack];
extern int topoStack;

// Declaração de funções
void imprimir_comandos();
void imprimirTabuleiro(char **tabuleiro, int linhas, int colunas);

void pintarDeBranco(char **tabuleiro, int linhas, int colunas, int x, int y);
void riscar(char **tabuleiro, int linhas, int colunas, int x, int y);
void aplicar_correcoes(Tabuleiro *t);
int resolve_jogo(Tabuleiro *t);
void lerJogo(char *nome, Tabuleiro *t);
void gravarJogo(char *nomeTabuleiro, Tabuleiro *t, char *nomeStack);
void guardar_move(char ação, int x, int y, char ant_val, char novo_val);
void desfazer(Tabuleiro *t);
int formatoParaCoordenadas(char *input, int *x, int *y);
void gravarStack(char *nome);
void lerStack(char *nome);
void verificar_riscadas(Tabuleiro *t);
void verificar_brancas(Tabuleiro *t);
void verificar_estado(Tabuleiro *t);
Tabuleiro copiar_tabuleiro(Tabuleiro *t);
void comando_R(Tabuleiro *t); 
void consumirNovaLinha(FILE *f);
Tabuleiro criar_tabuleiro(int l, int c);
void libertar_tabuleiro(Tabuleiro* t);
bool resolver_recursivo(Tabuleiro* t);
bool validacao_parcial(Tabuleiro* t, int linha, int coluna);
bool verificar_conectividade(Tabuleiro* t);
bool tem_minusculas(Tabuleiro *t);
bool tabuleiros_iguais(Tabuleiro *a, Tabuleiro *b);

#endif
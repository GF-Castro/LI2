#ifndef PROJETO_H
#define PROJETO_H
#include <stdbool.h>
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
void aplicar_correcoes(Tabuleiro *t);
int resolve_jogo(Tabuleiro *t);
void lerJogo(char *nome, Tabuleiro *t);
void gravarJogo(char *nome, Tabuleiro *t);
void guardar_estado(Tabuleiro *t);
void desfazer(Tabuleiro *t);
int formatoParaCoordenadas(char *input, int *x, int *y);
Tabuleiro desempilhar();
void stacks(Tabuleiro estado);
void gravarStack(char *nome);
void lerStack(char *nome);
void verificar_riscadas(Tabuleiro *t);
void verificar_brancas(Tabuleiro *t);
bool verificar_conectividade(Tabuleiro *t);
void verificar_estado(Tabuleiro *t);
bool conexao_valida_apos_risco(Tabuleiro *orig, int ri, int rj);
int inferencia_inicial(Tabuleiro *t);
bool busca_solucao(Tabuleiro *t);
Tabuleiro copia_tabuleiro(Tabuleiro *t) ;
bool violacao_basica(Tabuleiro *t);
bool completo(Tabuleiro *t);
void comando_R(Tabuleiro *t); 



#endif


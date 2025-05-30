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

// Declaração de variáveis globais, usei extern para evitar duplicação
extern Move movestack[tamanhoStack];
extern int topoStack;

// Declaração de funções
void imprimir_comandos();
void imprimirTabuleiro(char **tabuleiro, int linhas, int colunas);

void pintarDeBranco(char **tabuleiro, int linhas, int colunas, int x, int y);
void riscar(char **tabuleiro, int linhas, int colunas, int x, int y);
int pintar_vizinhos_de_branco(Tabuleiro *t, int i, int j);
void aplicar_correcoes(Tabuleiro *t);
int resolve_jogo(Tabuleiro *t);
void lerJogo(char *nome, Tabuleiro *t);
void gravarJogo(char *nomeTabuleiro, Tabuleiro *t, char *nomeStack);
void guardar_move(char action, int x, int y, char prev_val, char new_val);
void desfazer(Tabuleiro *t);
int formatoParaCoordenadas(char *input, int *x, int *y);
Move desempilhar();
void stacks(Move movimento);
void gravarStack(char *nome);
void lerStack(char *nome);
void verificar_riscadas(Tabuleiro *t);
void verificar_brancas(Tabuleiro *t);
void verificar_estado(Tabuleiro *t);
bool e_possivel_resolver (Tabuleiro *t);
bool conexao_valida_apos_risco(Tabuleiro *orig, int ri, int rj);
int inferencia_inicial(Tabuleiro *t);
Tabuleiro copiar_tabuleiro(Tabuleiro *t);
bool violacao_basica(Tabuleiro *t);
bool completo(Tabuleiro *t);
void comando_R(Tabuleiro *t); 
void consumirNovaLinha(FILE *f);
bool busca_solucao(Tabuleiro *t);
Tabuleiro criar_tabuleiro(int l, int c);
void libertar_tabuleiro(Tabuleiro* t);
bool resolver_recursivo(Tabuleiro* t);
bool validacao_parcial(Tabuleiro* t, int linha, int coluna);
bool verificar_conectividade(Tabuleiro* t);
Tabuleiro limpar_tabuleiro(Tabuleiro *t);
bool tem_minusculas(Tabuleiro *t);
bool tabuleiros_iguais(Tabuleiro *a, Tabuleiro *b);

#endif
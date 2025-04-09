#ifndef PROJETO_H
#define PROJETO_H

void lerEstadoJogo(char *nomearquivo, char tabuleiro[1000][1000], int *linhas, int *colunas);
void imprimirTabuleiro(char tabuleiro[1000][1000], int linhas, int colunas);
void pintarDeBranco(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void riscar(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y);
void imprimir_comandos();
int formatoParaCoordenadas(char *input, int *x, int *y);

#endif
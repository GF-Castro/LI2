#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Declarações globais
Tabuleiro stack[tamanhoStack];
int topoStack = -1;

/*int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (strlen(input) < 2 || !isalpha(input[0]) || !isdigit(input[1])) {
        return 0;
    }
    *x = input[0] - 'a';
    *y = input[1] - '1';
    return 1;﻿
}
*/

int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (input == NULL || strlen(input) < 2) return 0;

    if (!isalpha(input[0])) return 0;

    // Converte letra para índice de linha, aceita maiúsculas e minúsculas
    *x = tolower(input[0]) - 'a';

    // Verifica se o resto é só número (sem caracteres extras)
    char *endptr;
    long numero = strtol(input + 1, &endptr, 10);

    if (*endptr != '\0') return 0;  // Há caracteres a mais após o número
    if (numero < 1) return 0;       // Número deve ser positivo

    *y = (int)numero - 1;
    return 1;
}

// Funções relacionadas ao tabuleiro
void imprimirTabuleiro(char tabuleiro[1000][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void pintarDeBranco(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        tabuleiro[y][x] = toupper(tabuleiro[y][x]);
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void riscar(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        if (tabuleiro[y][x] != '\0') {
            tabuleiro[y][x] = '#';
        } else {
            printf("Célula vazia, não pode ser riscada.\n");
        }
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

// Funções de gerenciamento de estado
void stacks(Tabuleiro estado) {
    if (topoStack < tamanhoStack - 1) {
        topoStack++;
        stack[topoStack] = estado;
    } else {
        printf("Stack cheia. Não é possível guardar mais estados.\n");
    }
}

Tabuleiro desempilhar() {
    if (topoStack >= 0) {
        return stack[topoStack--];
    } else {
        printf("Stack vazia.\n");
        Tabuleiro vazio = {{{0}}, 0, 0};
        return vazio;
    }
}

void guardar_estado(Tabuleiro *t) {
    stacks(*t);
}

void desfazer(Tabuleiro *t) {
    Tabuleiro tabuleiroAnterior = desempilhar();
    if (tabuleiroAnterior.linhas > 0 && tabuleiroAnterior.colunas > 0) {
        *t = tabuleiroAnterior;
   // Funções auxiliares
    } else {
        printf("Não é possível desfazer mais ações.\n");
    }
}

// Funções de gravação e leitura
void gravarStack(char *nome) {
    FILE *f = fopen(nome, "w");
    if (!f) {
        perror("Erro ao guardar a stack");
        return;
    }

    fprintf(f, "%d\n", topoStack);

    for (int i = 0; i <= topoStack; i++) {
        fprintf(f, "%d %d\n", stack[i].linhas, stack[i].colunas);
        for (int y = 0; y < stack[i].linhas; y++) {
            for (int x = 0; x < stack[i].colunas; x++) {
                fputc(stack[i].tabuleiro[y][x], f);
            }
            fputc('\n', f);
        }
    }

    fclose(f);
}

void gravarJogo(char *nome, Tabuleiro *t) {
    FILE *arquivo = fopen(nome, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir tabuleiro.txt");
        return;
    }

    fprintf(arquivo, "%d %d\n", t->colunas, t->linhas);

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            fputc(t->tabuleiro[i][j], arquivo);
        }
        fputc('\n', arquivo);
    }

    fclose(arquivo);
    gravarStack("stack.txt");
}

void lerStack(char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        topoStack = -1;
        return;
    }

    if (fscanf(f, "%d\n", &topoStack) != 1) {
        fclose(f);
        topoStack = -1;
        return;
    }

    for (int i = 0; i <= topoStack; i++) {
        if (fscanf(f, "%d %d\n", &stack[i].linhas, &stack[i].colunas) != 2) {
            topoStack = i - 1;
            break;
        }

        for (int y = 0; y < stack[i].linhas; y++) {
            for (int x = 0; x < stack[i].colunas; x++) {
                stack[i].tabuleiro[y][x] = fgetc(f);
            }
            fgetc(f);
        }
    }

    fclose(f);
}

void lerJogo(char *nome, Tabuleiro *t) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro: %s\n", nome);
        return;
    }

    if (fscanf(f, "%d %d", &t->colunas, &t->linhas)!=2){
        printf("Erro a ler tabuleiro.\n");
    };
    fgetc(f);

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            t->tabuleiro[i][j] = fgetc(f);
        }
        fgetc(f);
    }

    fclose(f);
    lerStack("stack.txt");
}

// Funções de verificação e regras
void verificar_riscadas(char tabuleiro[1000][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == '#') {
                int brancas = 0;

                if (i > 0 && isupper(tabuleiro[i-1][j])) brancas++;
                if (i < linhas-1 && isupper(tabuleiro[i+1][j])) brancas++;
                if (j > 0 && isupper(tabuleiro[i][j-1])) brancas++;
                if (j < colunas-1 && isupper(tabuleiro[i][j+1])) brancas++;

                if (brancas < 4) {
                    printf("Restrição violada: Casa #%d,%d não está rodeada por 4 casas brancas.\n", i, j);
                }
            }
        }
    }
}

void verificar_brancas(char tabuleiro[1000][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        int letras[26] = {0};
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j])) {
                int idx = tabuleiro[i][j] - 'A';
                letras[idx]++;
                if (letras[idx] > 1) {
                    printf("Restrição violada: Letra %c repetida na linha %d.\n", tabuleiro[i][j], i);
                }
            }
        }
    }

    for (int j = 0; j < colunas; j++) {
        int letras[26] = {0};
        for (int i = 0; i < linhas; i++) {
            if (isupper(tabuleiro[i][j])) {
                int idx = tabuleiro[i][j] - 'A';
                letras[idx]++;
                if (letras[idx] > 1) {
                    printf("Restrição violada: Letra %c repetida na coluna %d.\n", tabuleiro[i][j], j);
                }
            }
        }
    }
}

void verificar_estado(char tabuleiro[1000][1000], int linhas, int colunas) {
    printf("Verificando restrições...\n");
    verificar_riscadas(tabuleiro, linhas, colunas);
    verificar_brancas(tabuleiro, linhas, colunas);
    printf("Verificação concluída.\n");
}

// Funções de interface
void imprimir_comandos() {
    printf("Comandos disponíveis:\n");
    printf("g - Gravar o jogo atual no ficheiro\n");
    printf("l - Ler o estado do jogo num ficheiro\n");
    printf("b letra nr - Pintar de branco\n");
    printf("r letra nr - Riscar\n");
    printf("v - Verificar restrições\n");
    printf("a - Aplicar inferências\n");
    printf("A - Repetir inferências até não haver mudanças\n");
    printf("R - Resolver o jogo\n");
    printf("d - Desfazer última ação\n");
    printf("s - Sair\n");
}
#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
        if (tabuleiro[y][x] != '\0') {  // Verifica se a célula está inicializada
            tabuleiro[y][x] = '#';
        } else {
            printf("Célula vazia, não pode ser riscada.\n");
        }
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

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

int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (strlen(input) < 2 || !isalpha(input[0]) || !isdigit(input[1])) {
        return 0;
    }
    *x = input[0] - 'a';
    *y = input[1] - '1';
    return 1;
}

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

void gravarJogo(char *nome, Tabuleiro *t) {
    guardar_estado(t);  // salva o estado atual antes de gravar

    FILE *f = fopen(nome, "w");
    if (!f) {
        printf("Erro ao abrir ficheiro para escrita: %s\n", nome);
        return;
    }

    // escreve dimensões
    fprintf(f, "%d %d\n", t->linhas, t->colunas);

    // escreve o tabuleiro
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            fputc(t->tabuleiro[i][j], f);
        }
        fputc('\n', f);
    }

    fclose(f);
    printf("Jogo gravado com sucesso em '%s'.\n", nome);
}

void lerJogo(char *nome, Tabuleiro *t) {
    guardar_estado(t);  // para permitir desfazer o 'l'

    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro: %s\n", nome);
        return;
    }

    fscanf(f, "%d %d", &t->colunas, &t->linhas);
    fgetc(f); // consome newline

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            t->tabuleiro[i][j] = fgetc(f);
        }
        fgetc(f); // consome newline
    }

    fclose(f);
}

void verificar_riscadas(char tabuleiro[1000][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == '#') {
                int brancas = 0;

                // Cima
                if (i > 0 && isupper(tabuleiro[i-1][j])) brancas++;
                // Baixo
                if (i < linhas-1 && isupper(tabuleiro[i+1][j])) brancas++;
                // Esquerda
                if (j > 0 && isupper(tabuleiro[i][j-1])) brancas++;
                // Direita
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

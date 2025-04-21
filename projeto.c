#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

Tabuleiro stack[tamanhoStack];
int topoStack = -1;

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

void gravarStack(char *nome) {
    FILE *f = fopen(nome, "w");
    if (!f) {
        perror("Erro ao guardar a stack");
        return;
    }

    fprintf(f, "%d\n", topoStack);  // primeiro guarda o topo da stack

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

    // Grava a stack juntamente com o tabuleiro atual 
    gravarStack("stack.txt");
}
void guardar_estado(Tabuleiro *t) {
    stacks(*t);
}

void desfazer(Tabuleiro *t) {
    Tabuleiro tabuleiroAnterior = desempilhar();
    if (tabuleiroAnterior.linhas > 0 && tabuleiroAnterior.colunas > 0) {
        *t = tabuleiroAnterior;
    } else {
        printf("Não é possível desfazer mais ações.\n");
    }
}

void lerStack(char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        topoStack = -1;
        return;
    }

    if (fscanf(f, "%d\n", &topoStack) != 1) {
        // Pode ser um novo jogo, não mostrar erro
        fclose(f);
        topoStack = -1;
        return;
    }

    for (int i = 0; i <= topoStack; i++) {
        if (fscanf(f, "%d %d\n", &stack[i].linhas, &stack[i].colunas) != 2) {
            // Leitura incompleta ou corrompida, ajusta topo
            topoStack = i - 1;
            break;
        }

        for (int y = 0; y < stack[i].linhas; y++) {
            for (int x = 0; x < stack[i].colunas; x++) {
                stack[i].tabuleiro[y][x] = fgetc(f);
            }
            fgetc(f); // consumir '\n'
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

    fscanf(f, "%d %d", &t->colunas, &t->linhas);
    fgetc(f);

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            t->tabuleiro[i][j] = fgetc(f);
        }
        fgetc(f);
    }

    fclose(f);

    // Queremos carregar a stack também para podermos desfazer ações mesmo depois de carregar o jogo e não ter uma stack vazia
    lerStack("stack.txt");
}

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
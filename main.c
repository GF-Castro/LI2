#include "projeto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Usei Malloc e Free

#define STACK_SIZE 100

typedef struct {
    char tabuleiro[1000][1000];
    int linhas;
    int colunas;
} Tabuleiro;

// Stack para armazenar o tabuleiro 
Tabuleiro stack[STACK_SIZE];
int topoStack = -1;

// Função para empilhar um estado do tabuleiro para o topo da stack
void stacks(Tabuleiro state) {
    if (topoStack < STACK_SIZE - 1) {
        stack[++topoStack] = state;
    } else {
        printf("Erro: Pilha cheia, não é possível salvar o estado.\n");
    }
}

// Função para desempilhar um estado do topo da pilha
Tabuleiro desempilhar() {
    if (topoStack >= 0) {
        return stack[topoStack--]; // Retorna o tabuleiro do topo e reduz o topo o indice do topo da stack
    } else {
        printf("Erro: Pilha vazia, não há estado anterior.\n");
        
        // Devolve um tabuleiro vazio para o caso da stack estar vazia
        Tabuleiro tabuleiroVazio = {{{0}}, 0, 0}; 
        return tabuleiroVazio;
    }
}

int main() {
    char comando[1000];
    char tabuleiro[1000][1000];
    int linhas = 0, colunas = 0;
    int x, y;
    char coord[10];

    imprimir_comandos();

    while (1) {
        printf("Comando: ");
        if (scanf("%s", comando) != 1) {
            printf("Erro a ler comando.\n");
            continue;
        }

        if (strcmp(comando, "l") == 0) {
            lerEstadoJogo("tabuleiro.txt", tabuleiro, &linhas, &colunas);
            imprimirTabuleiro(tabuleiro, linhas, colunas);
        } else if (strcmp(comando, "s") == 0) {
            printf("Jogo encerrado.\n");
            break;
        } else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            if (scanf("%s", coord) != 1) {
                printf("Erro a ler coordenada.\n");
                continue;
            }

            if (!formatoParaCoordenadas(coord, &x, &y)) {
                printf("Coordenada inválida. Use formato como 'c3'.\n");
                continue;
            }

            // Guarda o tabuleiro atual numa stack
            Tabuleiro currentState;
            memcpy(currentState.tabuleiro, tabuleiro, sizeof(tabuleiro));
            currentState.linhas = linhas;
            currentState.colunas = colunas;
            stacks(currentState);

            if (strcmp(comando, "b") == 0) {
                pintarDeBranco(tabuleiro, linhas, colunas, x, y);
            } else {
                riscar(tabuleiro, linhas, colunas, x, y);
            }

            imprimirTabuleiro(tabuleiro, linhas, colunas);
        } else if (strcmp(comando, "d") == 0) {
            if (topoStack == -1) {
                printf("Nenhuma ação para desfazer.\n");
                continue;
            }

            // Desfaz a última ação
            Tabuleiro previousState = desempilhar();
            memcpy(tabuleiro, previousState.tabuleiro, sizeof(tabuleiro));
            linhas = previousState.linhas;
            colunas = previousState.colunas;

            imprimirTabuleiro(tabuleiro, linhas, colunas);
        } else {
            printf("Comando inválido.\n");
        }
    }

    return 0;
}

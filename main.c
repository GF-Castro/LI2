#include "projeto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Função auxiliar para comparar dois tabuleiros (deep compare)
bool tabuleiros_iguais(Tabuleiro *a, Tabuleiro *b) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) return false;
    for (int i = 0; i < a->linhas; i++)
        for (int j = 0; j < a->colunas; j++)
            if (a->tabuleiro[i][j] != b->tabuleiro[i][j])
                return false;
    return true;
}

int main() {
    char comando[1000];
    char coord[10];
    int x, y;
    Tabuleiro t;

    imprimir_comandos();

    while (1) {
        printf("Comando: ");
        if (scanf("%s", comando) != 1) {
            printf("Erro a ler comando.\n");
            continue;
        }

        if (strcmp(comando, "l") == 0) {
            lerJogo("tabuleiro.txt", &t);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "g") == 0) {
            gravarJogo("tabuleiro.txt", &t);
        }
        else if (strcmp(comando, "s") == 0) {
            printf("Jogo encerrado.\n");
            break;
        }
        else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            if (scanf("%s", coord) != 1) {
                printf("Erro a ler coordenada.\n");
                continue;
            }
            if (!formatoParaCoordenadas(coord, &x, &y)) {
                printf("Coordenada inválida. Use formato como 'c3'.\n");
                continue;
            }

            // NÃO é preciso guardar_move aqui, pois pintarDeBranco/riscar já fazem isso

            if (strcmp(comando, "b") == 0)
                pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, x, y);
            else
                riscar(t.tabuleiro, t.linhas, t.colunas, x, y);

            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "d") == 0) {
            desfazer(&t);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "v") == 0) {
            verificar_estado(&t);
        }
        else if (strcmp(comando, "a") == 0) {
            // Para ações complexas, ainda podes querer guardar todos os movimentos feitos durante aplicar_correcoes
            aplicar_correcoes(&t);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "R") == 0) {
            if (e_possivel_resolver(&t)) {
                comando_R(&t);
            } else {
                printf("Erro: o tabuleiro atual viola as regras e não pode ser resolvido.\n");
            }
        }
        else if (strcmp(comando, "A") == 0) {
            do {
                Tabuleiro copia = copia_tabuleiro(&t);
                aplicar_correcoes(&t);
                if (tabuleiros_iguais(&copia, &t)) {
                    libertar_tabuleiro(&copia);
                    break;
                }
                libertar_tabuleiro(&copia);
            } while (1);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else {
            printf("Comando inválido. Tente novamente.\n");
            imprimir_comandos();
        }
    }

    libertar_tabuleiro(&t);

    return 0;
}
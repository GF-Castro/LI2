#include "projeto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main() {
    char linha[1024];
    char comando[100];
    char argumento[200];
    int num_args, x, y;
    Tabuleiro t;
    t.tabuleiro = NULL;
    t.linhas = t.colunas = 0;

    char current_filename[256] = "";

    imprimir_comandos();

    int sair = 0;

    while (!sair) {
        printf("Comando: ");
        if (!fgets(linha, sizeof(linha), stdin)) {
            sair = 1; // EOF ou erro
        } else {
            size_t len = strlen(linha);
            if (len > 0 && (linha[len-1] == '\n' || linha[len-1] == '\r')) {
                linha[--len] = '\0';
                if (len > 0 && (linha[len-1] == '\r' || linha[len-1] == '\n')) {
                    linha[--len] = '\0';
                }
            }

            num_args = sscanf(linha, "%s %s", comando, argumento);

            if (strcmp(comando, "l") == 0) {
                if (num_args >= 2) {
                    if (t.tabuleiro != NULL) {
                        libertar_tabuleiro(&t);
                        t.tabuleiro = NULL;
                    }
                    lerJogo(argumento, &t);
                    if (t.tabuleiro != NULL) {
                        imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
                        strncpy(current_filename, argumento, sizeof(current_filename)-1);
                        current_filename[sizeof(current_filename)-1] = '\0';
                    }
                    char nomeStack[256];
                    strcpy(nomeStack, "stack_");
                    strncat(nomeStack, current_filename, sizeof(nomeStack) - strlen(nomeStack) - 1);
                    lerStack(nomeStack);
                } else {
                    printf("Erro: comando 'l' requer nome do ficheiro (ex: l tabuleiro1.txt)\n");
                }

            } else if (strcmp(comando, "g") == 0) {
                if (t.tabuleiro != NULL) {
                    if (num_args >= 2) {
                        snprintf(current_filename, sizeof(current_filename), "%s.txt", argumento);
                    }
                    char nomeTabuleiro[256], nomeStack[256];
                    snprintf(nomeTabuleiro, sizeof(nomeTabuleiro), "%s", current_filename);
                    strcpy(nomeStack, "stack_");
                    strncat(nomeStack, current_filename, sizeof(nomeStack) - strlen(nomeStack) - 1);
                    gravarJogo(nomeTabuleiro, &t, nomeStack);
                    printf("Jogo gravado em '%s' e pilha em '%s'.\n", nomeTabuleiro, nomeStack);
                } else {
                    printf("Erro: não há tabuleiro para gravar. Use 'l <ficheiro>'.\n");
                }

            } else if (strcmp(comando, "s") == 0) {
                printf("Jogo encerrado.\n");
                sair = 1;

            } else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
                if (t.tabuleiro != NULL) {
                    if (num_args >= 2) {
                        if (formatoParaCoordenadas(argumento, &x, &y)) {
                            if (strcmp(comando, "b") == 0) {
                                pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, x, y);
                            } else {
                                riscar(t.tabuleiro, t.linhas, t.colunas, x, y);
                            }
                            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
                        } else {
                            printf("Coordenada inválida. Use formato como 'c3'.\n");
                        }
                    } else {
                        printf("Erro: comando '%s' requer coordenada (ex: %s c3)\n", comando, comando);
                    }
                } else {
                    printf("Erro: não carregou nenhum tabuleiro. Use 'l <ficheiro>'.\n");
                }

            } else if (strcmp(comando, "d") == 0) {
                if (t.tabuleiro != NULL) {
                    desfazer(&t);
                    imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
                } else {
                    printf("Erro: não há tabuleiro para desfazer.\n");
                }

            } else if (strcmp(comando, "v") == 0) {
                if (t.tabuleiro != NULL) {
                    verificar_estado(&t);
                } else {
                    printf("Erro: não há tabuleiro carregado.\n");
                }

            } else if (strcmp(comando, "a") == 0) {
                if (t.tabuleiro != NULL) {
                    aplicar_correcoes(&t);
                    imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
                } else {
                    printf("Erro: não há tabuleiro carregado.\n");
                }

            } else if (strcmp(comando, "R") == 0) {
                if (t.tabuleiro != NULL) {
                    comando_R(&t);
                } else {
                    printf("Erro: não há tabuleiro carregado.\n");
                }

            } else if (strcmp(comando, "A") == 0) {
                if (t.tabuleiro != NULL) {
                    Tabuleiro copia = copiar_tabuleiro(&t);
                    aplicar_correcoes(&t);
            
                    while (!tabuleiros_iguais(&copia, &t)) {
                        libertar_tabuleiro(&copia);
                        copia = copiar_tabuleiro(&t);
                        aplicar_correcoes(&t);
                    }
            
                    libertar_tabuleiro(&copia);
                    imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
            
                    if (tem_minusculas(&t)) {
                        printf(
                            "Atenção: o comando 'A' não conseguiu eliminar todas as minúsculas.\n"
                            "Ainda existem células por decidir que precisam de ação manual.\n"
                        );
                    }
                } else {
                    printf("Erro: não há tabuleiro carregado.\n");
                }
            } else {
                printf("Comando inválido.\n");
                imprimir_comandos();
            }
        }
    }

    if (t.tabuleiro != NULL) {
        libertar_tabuleiro(&t);
    }
    return 0;
}
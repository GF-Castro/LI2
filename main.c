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
    char linha[1024];
    char comando[100];
    char argumento[200];
    int num_args, x, y;
    Tabuleiro t;
    t.tabuleiro = NULL;
    t.linhas = t.colunas = 0;

    // __Variável para lembrar qual foi o ficheiro atualmente carregado__
    // Usaremos o nome completo (ex: "tabuleiro3.txt").
    char current_filename[256] = "";  

    imprimir_comandos();

    while (1) {
        printf("Comando: ");

        if (!fgets(linha, sizeof(linha), stdin)) {
            // EOF ou erro de leitura → sai do laço
            break;
        }

        // Remove eventual '\n' ou '\r' no final
        size_t len = strlen(linha);
        if (len > 0 && (linha[len-1] == '\n' || linha[len-1] == '\r')) {
            linha[len-1] = '\0';
            if (len > 1 && (linha[len-2] == '\r' || linha[len-2] == '\n')) {
                // No caso de CRLF
                linha[len-2] = '\0';
            }
        }

        // Faz parsing: comando e, possivelmente, um argumento
        num_args = sscanf(linha, "%s %s", comando, argumento);

        if (strcmp(comando, "l") == 0) {
            // comando 'l <ficheiro>'
            if (num_args < 2) {
                printf("Erro: comando 'l' requer nome do ficheiro (ex: l tabuleiro1.txt)\n");
                continue;
            }
            // Se já havia um tabuleiro carregado, liberta antes
            if (t.tabuleiro != NULL) {
                libertar_tabuleiro(&t);
                t.tabuleiro = NULL;
            }
            // Carrega o tabuleiro do ficheiro indicado
            lerJogo(argumento, &t);
            if (t.tabuleiro != NULL) {
                imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
                // Atualiza o nome do ficheiro atual
                strncpy(current_filename, argumento, sizeof(current_filename));
                current_filename[sizeof(current_filename)-1] = '\0';
            }
            // Carrega também a pilha de movimentos correspondente:
            {
                char nomeStack[256];
                snprintf(nomeStack, sizeof(nomeStack), "stack_%s", current_filename);
                lerStack(nomeStack);
            }
        }
        else if (strcmp(comando, "g") == 0) {
            // comando 'g' ou 'g <novoNome>'
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro para gravar. Use 'l <ficheiro>'.\n");
                continue;
            }

            // Se foi dado um argumento extra (um novo ficheiro), usamos esse
            if (num_args >= 2) {
                // O user digitou "g outroNome" → gravamos em "outroNome.txt"
                // Atualizamos current_filename para "outroNome.txt"
                snprintf(current_filename, sizeof(current_filename), "%s.txt", argumento);
            }
            // Se não foi dado argumento, current_filename já contém o ficheiro carregado

            // Agora construímos os nomes:
            // current_filename já é algo como "tabuleiro3.txt"
            char nomeTabuleiro[256], nomeStack[256];
            // Garantimos que current_filename termina em ".txt"
            // Se current_filename já tiver a extensão, não alteramos. 
            // (Aqui assumimos que o utilizador usou ".txt" ao carregar.)
            snprintf(nomeTabuleiro, sizeof(nomeTabuleiro), "%s", current_filename);
            // Para a stack, prefixamos "stack_"
            snprintf(nomeStack, sizeof(nomeStack), "stack_%s", current_filename);

            gravarJogo(nomeTabuleiro, &t, nomeStack);
            printf("Jogo gravado em '%s' e pilha em '%s'.\n",
                   nomeTabuleiro, nomeStack);
        }
        else if (strcmp(comando, "s") == 0) {
            // comando 's' → sair
            printf("Jogo encerrado.\n");
            break;
        }
        else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            // comandos 'b <coord>' ou 'r <coord>'
            if (t.tabuleiro == NULL) {
                printf("Erro: não carregou nenhum tabuleiro. Use 'l <ficheiro>'.\n");
                continue;
            }
            if (num_args < 2) {
                printf("Erro: comando '%s' requer coordenada (ex: %s c3)\n",
                       comando, comando);
                continue;
            }
            if (!formatoParaCoordenadas(argumento, &x, &y)) {
                printf("Coordenada inválida. Use formato como 'c3'.\n");
                continue;
            }
            if (strcmp(comando, "b") == 0) {
                pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, x, y);
            } else {
                riscar(t.tabuleiro, t.linhas, t.colunas, x, y);
            }
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "d") == 0) {
            // comando 'd' → desfazer
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro para desfazer.\n");
                continue;
            }
            desfazer(&t);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "v") == 0) {
            // comando 'v' → verificar estado
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro carregado.\n");
                continue;
            }
            verificar_estado(&t);
        }
        else if (strcmp(comando, "a") == 0) {
            // comando 'a' → aplicar inferências
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro carregado.\n");
                continue;
            }
            aplicar_correcoes(&t);
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else if (strcmp(comando, "R") == 0) {
            // comando 'R' → resolver
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro carregado.\n");
                continue;
            }
            if (e_possivel_resolver(&t)) {
                comando_R(&t);
            } else {
                printf("Erro: tabuleiro atual viola regras e não pode ser resolvido.\n");
            }
        }
        else if (strcmp(comando, "A") == 0) {
            // comando 'A' → repetir inferências até estabilizar
            if (t.tabuleiro == NULL) {
                printf("Erro: não há tabuleiro carregado.\n");
                continue;
            }
            while (1) {
                Tabuleiro copia = copia_tabuleiro(&t);
                aplicar_correcoes(&t);
                if (tabuleiros_iguais(&copia, &t)) {
                    libertar_tabuleiro(&copia);
                    break;
                }
                libertar_tabuleiro(&copia);
            }
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        }
        else {
            printf("Comando inválido. Tente novamente.\n");
            imprimir_comandos();
        }
    }

    // Antes de terminar, liberta o tabuleiro se existir
    if (t.tabuleiro != NULL) {
        libertar_tabuleiro(&t);
    }
    return 0;
}
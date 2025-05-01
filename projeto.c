#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Declarações globais
Tabuleiro stack[tamanhoStack];
int topoStack = -1;

int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (input == NULL || strlen(input) < 2) return 0;

    if (!isalpha(input[0])) return 0;

    // Converte letra para índice de linha, aceita maiúsculas e minúsculas
    *x = tolower(input[0]) - 'a';

    // Verifica se o resto é só número (sem caracteres extras)
    char *ponteirofim;
    long numero = strtol(input + 1, &ponteirofim, 10);

    if (*ponteirofim != '\0') return 0;  // Há caracteres a mais após o número
    if (numero < 1) return 0;       // Número deve ser positivo

    *y = (int)numero - 1;
    return 1;
}

// Funções relacionadas ao tabuleiro
void imprimirTabuleiro(char tabuleiro[26][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void pintarDeBranco(char tabuleiro[26][1000], int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        tabuleiro[y][x] = toupper(tabuleiro[y][x]);
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void riscar(char tabuleiro[26][1000], int linhas, int colunas, int x, int y) {
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

// Função para transformar casas vizinhas de riscas em brancas, retornando se houve mudança
int pintar_vizinhos_de_branco(Tabuleiro *t, int i, int j) {
    int mudou = 0;
    if (i > 0 && islower((unsigned char)t->tabuleiro[i - 1][j])) {
        t->tabuleiro[i - 1][j] = toupper((unsigned char)t->tabuleiro[i - 1][j]);
        mudou = 1;
    }
    if (i < t->linhas - 1 && islower((unsigned char)t->tabuleiro[i + 1][j])) {
        t->tabuleiro[i + 1][j] = toupper((unsigned char)t->tabuleiro[i + 1][j]);
        mudou = 1;
    }
    if (j > 0 && islower((unsigned char)t->tabuleiro[i][j - 1])) {
        t->tabuleiro[i][j - 1] = toupper((unsigned char)t->tabuleiro[i][j - 1]);
        mudou = 1;
    }
    if (j < t->colunas - 1 && islower((unsigned char)t->tabuleiro[i][j + 1])) {
        t->tabuleiro[i][j + 1] = toupper((unsigned char)t->tabuleiro[i][j + 1]);
        mudou = 1;
    }
    return mudou;
}

int aplicar_inferencia(Tabuleiro *t) {
    int alterado;
    int total_mudou = 0;

    do {
        alterado = 0;

        for (int i = 0; i < t->linhas; i++) {
            for (int j = 0; j < t->colunas; j++) {
                char atual = t->tabuleiro[i][j];

                // Regra 1: riscar repetidos de brancas
                if (isupper((unsigned char)atual)) {
                    for (int k = 0; k < t->colunas; k++) {
                        if (k != j && t->tabuleiro[i][k] == atual) {
                            t->tabuleiro[i][k] = '#';
                            alterado = 1;
                        }
                    }
                    for (int k = 0; k < t->linhas; k++) {
                        if (k != i && t->tabuleiro[k][j] == atual) {
                            t->tabuleiro[k][j] = '#';
                            alterado = 1;
                        }
                    }
                }

                // Regra 2: vizinhos de # viram brancos
                if (t->tabuleiro[i][j] == '#') {
                    alterado |= pintar_vizinhos_de_branco(t, i, j);
                }

                // Regra 3: se riscar isolaria, pintar de branco
                if (islower((unsigned char)atual)) {
                    int pode_riscar = 0;
                    if (i > 0 && isupper((unsigned char)t->tabuleiro[i - 1][j])) pode_riscar = 1;
                    if (i < t->linhas - 1 && isupper((unsigned char)t->tabuleiro[i + 1][j])) pode_riscar = 1;
                    if (j > 0 && isupper((unsigned char)t->tabuleiro[i][j - 1])) pode_riscar = 1;
                    if (j < t->colunas - 1 && isupper((unsigned char)t->tabuleiro[i][j + 1])) pode_riscar = 1;

                    if (!pode_riscar) {
                        t->tabuleiro[i][j] = toupper((unsigned char)atual);
                        alterado = 1;
                    }
                }
            }
        }

        if (alterado) total_mudou = 1;

    } while (alterado);

    return total_mudou;
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
void verificar_riscadas(char tabuleiro[26][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == '#') {
                int brancas = 0;
                int vizinhas = 0;

                // Cima
                if (i > 0) {
                    vizinhas++;
                    if (isupper(tabuleiro[i-1][j])) brancas++;
                }
                // Baixo
                if (i < linhas-1) {
                    vizinhas++;
                    if (isupper(tabuleiro[i+1][j])) brancas++;
                }
                // Esquerda
                if (j > 0) {
                    vizinhas++;
                    if (isupper(tabuleiro[i][j-1])) brancas++;
                }
                // Direita
                if (j < colunas-1) {
                    vizinhas++;
                    if (isupper(tabuleiro[i][j+1])) brancas++;
                }

                // Agora comparamos brancas com o número de vizinhas existentes
                if (brancas < vizinhas) {
                    char coluna = 'a' + j;      
                    int  linha  = i + 1;        
                    
                    printf("Restrição violada: Casa %c%d não está rodeada por %d vizinhas brancas.\n",
                           coluna, linha, vizinhas);
                    
                }
            }
        }
    }
}


void verificar_brancas(char tabuleiro[26][1000], int linhas, int colunas) {
    // Verifica repetições em cada linha
    for (int i = 0; i < linhas; i++) {
        int contador[26] = {0};

        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j])) {
                contador[tabuleiro[i][j] - 'A']++;
            }
        }

        for (int k = 0; k < 26; k++) {
            if (contador[k] > 1) {
                char letra = 'A' + k;
                int linha_visual = i + 1;  
                printf("Restrição violada: Letra %c foi repetida %d vezes na linha %d.\n",
                       letra, contador[k], linha_visual);
            }
        }
    }
    // Verifica repetições em cada coluna
    for (int j = 0; j < colunas; j++) {
        int contador[26] = {0};
        for (int i = 0; i < linhas; i++) {
            if (isupper(tabuleiro[i][j])) {
                contador[tabuleiro[i][j] - 'A']++;
            }
        }

        for (int k = 0; k < 26; k++) {
            if (contador[k] > 1) {
                char letra = 'A' + k;
                char coluna_letra = 'a' + j; 
                printf("Restrição violada: Letra %c foi repetida %d vezes na coluna %c.\n",
                       letra, contador[k], coluna_letra);
            }
        }
    }
}


void verificar_estado(char tabuleiro[26][1000], int linhas, int colunas) {
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
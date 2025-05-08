#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Declaração do array de estados e topo da stack
Tabuleiro stack[tamanhoStack];  // Guarda os estados anteriores do tabuleiro
int topoStack = -1;             // Índice do topo da stack (inicialmente vazio)

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
    int alterado = 0;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++) {
        int ni = i + dx[d];
        int nj = j + dy[d];

        if (ni >= 0 && ni < t->linhas && nj >= 0 && nj < t->colunas) {
            char c = t->tabuleiro[ni][nj];
            if (islower((char)c)) {
                t->tabuleiro[ni][nj] = toupper((char)c);
                alterado = 1;
            }
        }
    }

    return alterado;
}

// Função que empilha um estado do tabuleiro
void stacks(Tabuleiro estado) {
    if (topoStack < tamanhoStack - 1) {      // Verifica se há espaço na stack
        topoStack++;                         // Avança o topo
        stack[topoStack] = estado;           // Guarda o novo estado
    } else {
        printf("Stack cheia. Não é possível guardar mais estados.\n");
    }
}

// Função que retira o último estado guardado (desfazer)
Tabuleiro desempilhar() {
    if (topoStack >= 0) {
        return stack[topoStack--];           // Retorna e remove o topo da stack
    } else {
        printf("Stack vazia.\n");
        Tabuleiro vazio = {{{0}}, 0, 0};     // Retorna tabuleiro vazio em caso de erro
        return vazio;
    }
}

// Função que guarda o estado atual do tabuleiro na stack
void guardar_estado(Tabuleiro *t) {
    stacks(*t);  // Copia o estado atual para a stack
}


// Função que desfaz a última ação, restaurando o tabuleiro anterior
void desfazer(Tabuleiro *t) {
    Tabuleiro tabuleiroAnterior = desempilhar();  // Recupera o estado anterior

    // Só substitui se o estado desempilhado for válido
    if (tabuleiroAnterior.linhas > 0 && tabuleiroAnterior.colunas > 0) {
        *t = tabuleiroAnterior;
    } else {
        printf("Não é possível desfazer mais ações.\n");
    }
}


// Grava a stack de estados num ficheiro
void gravarStack(char *nome) {
    FILE *f = fopen(nome, "w");
    if (!f) {
        perror("Erro ao guardar a stack");
        return;
    }

    fprintf(f, "%d\n", topoStack);  // Escreve topo da stack

    // Escreve cada estado da stack
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

// Grava o estado atual do jogo e a stack
void gravarJogo(char *nome, Tabuleiro *t) {
    FILE *arquivo = fopen(nome, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir tabuleiro.txt");
        return;
    }

    fprintf(arquivo, "%d %d\n", t->colunas, t->linhas);  // Grava dimensões

    // Grava tabuleiro linha por linha
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            fputc(t->tabuleiro[i][j], arquivo);
        }
        fputc('\n', arquivo);
    }

    fclose(arquivo);

    // Também grava a stack
    gravarStack("stack.txt");
}


// Lê o histórico da stack de um ficheiro
void lerStack(char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        topoStack = -1;
        return;
    }

    // Lê o topo
    if (fscanf(f, "%d\n", &topoStack) != 1) {
        fclose(f);
        topoStack = -1;
        return;
    }

    // Lê os estados
    for (int i = 0; i <= topoStack; i++) {
        if (fscanf(f, "%d %d\n", &stack[i].linhas, &stack[i].colunas) != 2) {
            topoStack = i - 1;
            break;
        }

        for (int y = 0; y < stack[i].linhas; y++) {
            for (int x = 0; x < stack[i].colunas; x++) {
                stack[i].tabuleiro[y][x] = fgetc(f);
            }
            fgetc(f); // Salta '\n'
        }
    }

    fclose(f);
}


// Lê o estado atual do tabuleiro de um ficheiro
void lerJogo(char *nome, Tabuleiro *t) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro: %s\n", nome);
        return;
    }

    // Lê dimensões
    if (fscanf(f, "%d %d", &t->linhas, &t->colunas) != 2) {
        printf("Erro ao ler dimensões do tabuleiro.\n");
        fclose(f);
        return;
    }

    fgetc(f);  // Consome '\n' após dimensões

    // Lê o conteúdo do tabuleiro
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            char c = fgetc(f);
            if (c == EOF) {
                printf("Erro: Dados insuficientes no ficheiro.\n");
                fclose(f);
                return;
            }
            t->tabuleiro[i][j] = c;
        }
        fgetc(f); // Salta '\n'
    }

    fclose(f);
}

// Verifica se todas as casas riscadas estão rodeadas de brancas
void verificar_riscadas(Tabuleiro *t) {
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (t->tabuleiro[i][j] == '#') {  // Se for uma casa riscada
                int brancas = 0, vizinhas = 0;

                // Cima
                if (i > 0) {
                    vizinhas++;
                    if (isupper(t->tabuleiro[i-1][j])) brancas++;
                }
                // Baixo
                if (i < t->linhas - 1) {
                    vizinhas++;
                    if (isupper(t->tabuleiro[i+1][j])) brancas++;
                }
                // Esquerda
                if (j > 0) {
                    vizinhas++;
                    if (isupper(t->tabuleiro[i][j-1])) brancas++;
                }
                // Direita
                if (j < t->colunas - 1) {
                    vizinhas++;
                    if (isupper(t->tabuleiro[i][j+1])) brancas++;
                }

                // Regra: casa riscada deve estar rodeada por brancas
                if (brancas < vizinhas) {
                    char col = 'a' + j;
                    int lin = i + 1;
                    printf("Restrição violada: Casa %c%d não está rodeada por %d vizinhas brancas.\n",
                           col, lin, vizinhas);
                }
            }
        }
    }
}

// Verifica se há letras brancas repetidas em linhas e colunas
void verificar_brancas(Tabuleiro *t) {
    // Verifica por linha
    for (int i = 0; i < t->linhas; i++) {
        int cnt[26] = {0};
        for (int j = 0; j < t->colunas; j++) {
            char c = t->tabuleiro[i][j];
            if (isupper(c)) cnt[c - 'A']++;
        }
        for (int k = 0; k < 26; k++) {
            if (cnt[k] > 1) {
                printf("Restrição violada: Letra %c foi repetida %d vezes na linha %d.\n",
                       'A' + k, cnt[k], i+1);
            }
        }
    }

    // Verifica por coluna
    for (int j = 0; j < t->colunas; j++) {
        int cnt[26] = {0};
        for (int i = 0; i < t->linhas; i++) {
            char c = t->tabuleiro[i][j];
            if (isupper(c)) cnt[c - 'A']++;
        }
        for (int k = 0; k < 26; k++) {
            if (cnt[k] > 1) {
                printf("Restrição violada: Letra %c foi repetida %d vezes na coluna %c.\n",
                       'A' + k, cnt[k], 'a' + j);
            }
        }
    }
}

// Verifica se todas as casas brancas estão conectadas
bool verificar_conectividade(Tabuleiro *t) {
    int L = t->linhas, C = t->colunas;
    bool visitado[26][1000];
    memset(visitado, 0, sizeof visitado);

    int total_brancas = 0;
    int comeco_i = -1, comeco_j = -1;

    // Procura todas as casas brancas e seleciona uma de partida
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            if (isupper(t->tabuleiro[i][j])) {
                total_brancas++;
                if (comeco_i < 0) {
                    comeco_i = i;
                    comeco_j = j;
                }
            }
        }
    }

    if (total_brancas <= 1) return true; // Conectividade trivial

    // BFS a partir da casa branca inicial
    int fila_i[L*C], fila_j[L*C], frente = 0, atras = 0;
    fila_i[atras] = comeco_i;
    fila_j[atras++] = comeco_j;
    visitado[comeco_i][comeco_j] = true;
    int cont = 1;

    int di[4] = {-1,1,0,0}, dj[4] = {0,0,-1,1};
    while (frente < atras) {
        int ci = fila_i[frente], cj = fila_j[frente++];
        for (int d = 0; d < 4; d++) {
            int ni = ci + di[d], nj = cj + dj[d];
            if (ni >= 0 && ni < L && nj >= 0 && nj < C &&
                isupper(t->tabuleiro[ni][nj]) && !visitado[ni][nj]) {
                visitado[ni][nj] = true;
                fila_i[atras] = ni;
                fila_j[atras++] = nj;
                cont++;
            }
        }
    }

    // Se todas as casas brancas foram visitadas, estão conectadas
    return cont == total_brancas;
}

// Função principal que junta todas as verificações de regras
void verificar_estado(Tabuleiro *t) {
    printf("Verificando restrições...\n");
    verificar_riscadas(t);         // Verifica casas riscadas
    verificar_brancas(t);          // Verifica repetições de letras
    if (!verificar_conectividade(t)) {
        printf("Restrição violada: há casas brancas isoladas no tabuleiro.\n");
    }
    printf("Verificação concluída.\n");
}


// Função auxiliar de conectividade
bool conexao_valida_apos_risco(Tabuleiro *orig, int ri, int rj) {
    // 1) Faz a cópia e aplica o risco
    Tabuleiro tmp = *orig;
    tmp.tabuleiro[ri][rj] = '#';

    // 2) Inicializa visitada
    bool visitada[26][1000];
    memset(visitada, 0, sizeof visitada);

    // 3) Conta apenas as casas brancas (maiúsculas) e escolhe ponto de partida
    int total = 0, comeco_i = -1, comeco_j = -1;
    for (int i = 0; i < tmp.linhas; i++) {
      for (int j = 0; j < tmp.colunas; j++) {
        if (isupper((char)tmp.tabuleiro[i][j])) {
          total++;
          if (comeco_i < 0) {
            comeco_i = i; comeco_j = j;
          }
        }
      }
    }

    // Se não houver ou houver só uma branca, conectividade é trivial
    if (total <= 1) return true;

    // 4) BFS só sobre as casas brancas
    int maxq = tmp.linhas * tmp.colunas;
    int fila_i[maxq], fila_j[maxq];
    int frente = 0, atras = 0;
    fila_i[atras] = comeco_i;
    fila_j[atras++] = comeco_j;
    visitada[comeco_i][comeco_j] = true;
    int cont = 1;

    int di[4] = {-1,1,0,0}, dj[4] = {0,0,-1,1};
    while (frente < atras) {
        int ci = fila_i[frente], cj = fila_j[frente++];
        for (int d = 0; d < 4; d++) {
            int ni = ci + di[d], nj = cj + dj[d];
            if (ni >= 0 && ni < tmp.linhas && nj >= 0 && nj < tmp.colunas
             && isupper((char)tmp.tabuleiro[ni][nj])
             && !visitada[ni][nj]) {
                visitada[ni][nj] = true;
                fila_i[atras] = ni;
                fila_j[atras++] = nj;
                cont++;
            }
        }
    }

    // 5) Só devolve true se visitou todas as brancas
    return cont == total;
}


// Aplica uma única vez as três regras de inferência (comando 'a'), usando sempre o tabuleiro original
void aplicar_correcoes(Tabuleiro *t) {
    Tabuleiro orig = *t;
    int alteracoes = 0;

    // Buffer para novo estado
    char novo[26][1000];
    for (int i = 0; i < orig.linhas; i++)
        for (int j = 0; j < orig.colunas; j++)
            novo[i][j] = orig.tabuleiro[i][j];

    // Regra 1: riscar em 'novo' minúsculas de cada branca em 'orig'
    for (int i = 0; i < orig.linhas; i++) {
        for (int j = 0; j < orig.colunas; j++) {
            if (isupper((char)orig.tabuleiro[i][j])) {
                char c = tolower(orig.tabuleiro[i][j]);
                for (int col = 0; col < orig.colunas; col++) {
                    if (orig.tabuleiro[i][col] == c && novo[i][col] != '#') {
                        novo[i][col] = '#'; alteracoes++;
                    }
                }
                for (int lin = 0; lin < orig.linhas; lin++) {
                    if (orig.tabuleiro[lin][j] == c && novo[lin][j] != '#') {
                        novo[lin][j] = '#'; alteracoes++;
                    }
                }
            }
        }
    }

    // Regra 2: pintar vizinhos de '#' em 'orig'
    int di[4] = {-1,1,0,0}, dj[4] = {0,0,-1,1};
    for (int i = 0; i < orig.linhas; i++) {
        for (int j = 0; j < orig.colunas; j++) {
            if (orig.tabuleiro[i][j] == '#') {
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d], nj = j + dj[d];
                    if (ni >= 0 && ni < orig.linhas && nj >= 0 && nj < orig.colunas
                        && islower(( char)orig.tabuleiro[ni][nj])
                        && novo[ni][nj] != toupper((char)orig.tabuleiro[ni][nj])) {
                        novo[ni][nj] = toupper(( char)orig.tabuleiro[ni][nj]);
                        alteracoes++;
                    }
                }
            }
        }
    }

    // Regra 3: evitar isolamento de brancas — usar conexão completa simulada
// Regra 3 corrigida: evitar isolamento E repetições
for (int i = 0; i < orig.linhas; i++) {
    for (int j = 0; j < orig.colunas; j++) {
        if (islower((char)orig.tabuleiro[i][j])) {
            // Verifica se riscar isolaria as brancas
            if (!conexao_valida_apos_risco(&orig, i, j)) {
                char mai = toupper((char)orig.tabuleiro[i][j]);
                
                // Verifica se a conversão para maiúscula é segura
                bool pode_pintar = true;
                
                // Verifica repetição na linha
                for (int x = 0; x < orig.colunas; x++) {
                    if (x != j && isupper(novo[i][x]) && novo[i][x] == mai) {
                        pode_pintar = false;
                        break;
                    }
                }
                
                // Verifica repetição na coluna
                if (pode_pintar) {
                    for (int y = 0; y < orig.linhas; y++) {
                        if (y != i && isupper(novo[y][j]) && novo[y][j] == mai) {
                            pode_pintar = false;
                            break;
                        }
                    }
                }
                
                // Aplica apenas se não houver repetições
                if (pode_pintar && novo[i][j] != mai) {
                    novo[i][j] = mai;
                    alteracoes++;
                }
            }
        }
    }
}
    // Copia 'novo' para o tabuleiro
    for (int i = 0; i < orig.linhas; i++)
        for (int j = 0; j < orig.colunas; j++)
            t->tabuleiro[i][j] = novo[i][j];

    printf("Alterações aplicadas: %d\n", alteracoes);
}

bool busca_solucao(Tabuleiro *t) {

    if (violacao_basica(t)) return false;

    if (completo(t)) {
        return verificar_conectividade(t); // Conectividade só no final
    }

    // 4) Escolha da célula
    int bi = -1, bj = -1;
    for (int i = 0; i < t->linhas && bi < 0; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (islower((char)t->tabuleiro[i][j])) {
                bi = i; bj = j; break;
            }
        }
    }

    // 5) Tenta pintar de branco
    {
        Tabuleiro copia = copia_tabuleiro(t);
        copia.tabuleiro[bi][bj] = toupper((char)copia.tabuleiro[bi][bj]);
        if (busca_solucao(&copia)) {
            *t = copia;
            return true;
        }
    }

    // 6) Tenta riscar
    {
        Tabuleiro copia = copia_tabuleiro(t);
        copia.tabuleiro[bi][bj] = '#';
        if (!violacao_basica(&copia)) { // Só continua se não houver '#' adjacentes
            if (busca_solucao(&copia)) {
                *t = copia;
                return true;
            }
        }
    }

    return false;
}

void comando_R(Tabuleiro *t) {
    Tabuleiro backup = copia_tabuleiro(t);

    if (!busca_solucao(t)) {
        printf("Não foi possível resolver o tabuleiro.\n");
        *t = backup;
        return;
    }

    printf("Solução encontrada:\n");
    imprimirTabuleiro(t->tabuleiro, t->linhas, t->colunas);
}

Tabuleiro copia_tabuleiro(Tabuleiro *t) {
    Tabuleiro c;
    c.linhas = t->linhas;
    c.colunas = t->colunas;
    for (int i = 0; i < t->linhas; i++) {
        memcpy(c.tabuleiro[i], t->tabuleiro[i], t->colunas);
    }
    return c;
}

// Verifica se há alguma violação das regras básicas do jogo
bool violacao_basica(Tabuleiro *t) {
    // 1a) Verificar repetições de letras brancas (maiúsculas) nas linhas
    for (int i = 0; i < t->linhas; i++) {
        int cnt[26] = {0}; // Contador de letras A-Z
        for (int j = 0; j < t->colunas; j++) {
            char c = t->tabuleiro[i][j];
            if (isupper((char)c)) { // Se for uma letra maiúscula
                if (++cnt[c - 'A'] > 1) return true; // Letra repetida → violação
            }
        }
    }

    // 1b) Verificar repetições de letras brancas (maiúsculas) nas colunas
    for (int j = 0; j < t->colunas; j++) {
        int cnt[26] = {0}; // Contador de letras A-Z
        for (int i = 0; i < t->linhas; i++) {
            char c = t->tabuleiro[i][j];
            if (isupper((char)c)) {
                if (++cnt[c - 'A'] > 1) return true; // Letra repetida → violação
            }
        }
    }

    // 1c) Verificar se cada célula riscada '#' tem pelo menos um vizinho branco
    int di[] = {-1, 1, 0, 0}; // deslocamentos para cima, baixo, esquerda, direita
    int dj[] = {0, 0, -1, 1};
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (t->tabuleiro[i][j] == '#') {
                bool tem_branco = false; // Flag para saber se há vizinho branco
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d];
                    int nj = j + dj[d];
                    // Verifica se a posição vizinha está dentro dos limites do tabuleiro
                    if (ni >= 0 && ni < t->linhas && nj >= 0 && nj < t->colunas) {
                        // Se há uma casa branca vizinha
                        if (isupper((char)t->tabuleiro[ni][nj])) {
                            tem_branco = true;
                            break;
                        }
                    }
                }
                if (!tem_branco) return true; // '#' isolado → violação
            }
        }
    }

    // 1d) Verificar se existem dois '#' adjacentes (não permitido)
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (t->tabuleiro[i][j] == '#') {
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d], nj = j + dj[d];
                    // Verifica se a posição vizinha está dentro do tabuleiro
                    if (ni >= 0 && ni < t->linhas && nj >= 0 && nj < t->colunas) {
                        if (t->tabuleiro[ni][nj] == '#') {
                            return true; // Dois '#' adjacentes → violação
                        }
                    }
                }
            }
        }
    }

    // Nenhuma violação encontrada
    return false;
}

// 2) Testa se o tabuleiro está completo (não há minúsculas)
bool completo(Tabuleiro *t) {
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (islower((char)t->tabuleiro[i][j]))
                return false;
        }
    }
    return true;
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

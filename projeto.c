#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Declaração do array de estados e topo da stack
Move movestack[tamanhoStack];  // Guarda os estados anteriores do tabuleiro
int topoStack = -1;             // Índice do topo da stack (inicialmente vazio)

int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (input == NULL || strlen(input) < 2) return 0;
    if (!isalpha(input[0])) return 0;
    *x = tolower(input[0]) - 'a';
    char *ponteirofim;
    long numero = strtol(input + 1, &ponteirofim, 10);
    if (*ponteirofim != '\0') return 0;
    if (numero < 1) return 0;
    *y = (int)numero - 1;
    return 1;
}

void imprimirTabuleiro(char **tabuleiro, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void pintarDeBranco(char **tabuleiro, int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        char prev = tabuleiro[y][x];
        char next = toupper(tabuleiro[y][x]);
        guardar_move('b', x, y, prev, next);
        tabuleiro[y][x] = next;
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void riscar(char **tabuleiro, int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        char prev = tabuleiro[y][x];
        char next = '#';
        guardar_move('r', x, y, prev, next);
        tabuleiro[y][x] = next;
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

// Função para empilhar um movimento (já existe guardar_move, mas podes ter stacks para compatibilidade)
void stacks(Move movimento) {
    if (topoStack < tamanhoStack - 1) {
        topoStack++;
        movestack[topoStack] = movimento;
    } else {
        printf("Stack cheia. Não é possível guardar mais movimentos.\n");
    }
}

// Função para desempilhar um movimento
Move desempilhar() {
    if (topoStack >= 0) {
        Move anterior = movestack[topoStack];
        topoStack--;
        return anterior;
    } else {
        printf("Stack vazia.\n");
        Move vazio = {'\0', -1, -1, '\0', '\0'};
        return vazio;
    }
}

void guardar_move(char action, int x, int y, char prev_val, char new_val) {
    if (topoStack < tamanhoStack - 1) {
        topoStack++;
        movestack[topoStack].action = action;
        movestack[topoStack].x = x;
        movestack[topoStack].y = y;
        movestack[topoStack].prev_val = prev_val;
        movestack[topoStack].new_val = new_val;
    } else {
        printf("Stack cheia. Não é possível guardar mais movimentos.\n");
    }
}

void desfazer(Tabuleiro *t) {
    if (topoStack < 0) {
        printf("Nada para desfazer.\n");
        return;
    }
    Move m = movestack[topoStack--];
    t->tabuleiro[m.y][m.x] = m.prev_val;
}

// Gravar/ler a pilha de movimentos
void gravarStack(char *nome) {
    FILE *f = fopen(nome, "w");
    if (!f) {
        perror("Erro ao guardar a stack");
        return;
    }
    // Grava a quantidade de movimentos (não o índice)
    fprintf(f, "%d\n", topoStack + 1);

    for (int i = 0; i <= topoStack; i++) {
        Move *m = &movestack[i];
        fprintf(f, "%c %d %d %c %c\n", m->action, m->x, m->y, m->prev_val, m->new_val);
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


void lerStack(char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        topoStack = -1;
        return;
    }
    int quantidade;
    if (fscanf(f, "%d\n", &quantidade) != 1) {
        fclose(f);
        topoStack = -1;
        return;
    }
    topoStack = quantidade - 1;
    for (int i = 0; i < quantidade; i++) {
        char action, prev_val, new_val;
        int x, y;
        if (fscanf(f, " %c %d %d %c %c\n", &action, &x, &y, &prev_val, &new_val) != 5) {
            topoStack = i - 1;
            break;
        }
        movestack[i].action = action;
        movestack[i].x = x;
        movestack[i].y = y;
        movestack[i].prev_val = prev_val;
        movestack[i].new_val = new_val;
    }
    fclose(f);
}

void consumirNovaLinha(FILE *f) {
    int ch;
    while ((ch = fgetc(f)) != EOF && ch != '\n');
}

void lerJogo(char *nome, Tabuleiro *t) {
    FILE *f = fopen(nome, "r");
    if (!f) { perror("abrir"); return; }

    int l, c;
    if (fscanf(f, "%d %d", &l, &c) != 2) { // l = linhas, c = colunas
        fprintf(stderr, "Dimensões inválidas\n");
        fclose(f);
        return;
    }
    consumirNovaLinha(f); // Consome tudo até ao '\n'

    *t = criar_tabuleiro(l, c); // Aloca l linhas e c colunas

    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            int ch = fgetc(f);
            t->tabuleiro[i][j] = (char)ch;
        }
        consumirNovaLinha(f); // Consome o '\n' no final de cada linha
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

bool e_possivel_resolver (Tabuleiro *t) {
    if (!verificar_conectividade(t)) return false;

    // Verifica repetição de letras em linhas
    for (int i = 0; i < t->linhas; i++) {
        int cnt[26] = {0};
        for (int j = 0; j < t->colunas; j++) {
            char c = t->tabuleiro[i][j];
            if (isupper(c)) cnt[c - 'A']++;
        }
        for (int k = 0; k < 26; k++) {
            if (cnt[k] > 1) return false;
        }
    }

    // Verifica repetição de letras em colunas
    for (int j = 0; j < t->colunas; j++) {
        int cnt[26] = {0};
        for (int i = 0; i < t->linhas; i++) {
            char c = t->tabuleiro[i][j];
            if (isupper(c)) cnt[c - 'A']++;
        }
        for (int k = 0; k < 26; k++) {
            if (cnt[k] > 1) return false;
        }
    }

    // Verifica casas riscadas rodeadas por brancas
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (t->tabuleiro[i][j] == '#') {
                int brancas = 0, vizinhas = 0;
                if (i > 0) { vizinhas++; if (isupper(t->tabuleiro[i-1][j])) brancas++; }
                if (i < t->linhas - 1) { vizinhas++; if (isupper(t->tabuleiro[i+1][j])) brancas++; }
                if (j > 0) { vizinhas++; if (isupper(t->tabuleiro[i][j-1])) brancas++; }
                if (j < t->colunas - 1) { vizinhas++; if (isupper(t->tabuleiro[i][j+1])) brancas++; }
                if (brancas < vizinhas) return false;
            }
        }
    }

    return true;
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
    // 1) Se violação básica → falha imediata
    if (violacao_basica(t)) 
        return false;

    // 2) Se completo, só falta verificar conectividade
    if (completo(t)) 
        return verificar_conectividade(t);

    // 3) Escolhe a célula (i,j) com letra minúscula
    int bi = -1, bj = -1;
    for (int i = 0; i < t->linhas && bi < 0; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (islower((unsigned char)t->tabuleiro[i][j])) {
                bi = i; bj = j;
                break;
            }
        }
    }
    // deveria sempre encontrar — mas em caso contrário falha
    if (bi < 0) return false;

    // 4) Tenta pintar de branco (maiúscula)
    {
        char orig = t->tabuleiro[bi][bj];
        t->tabuleiro[bi][bj] = toupper((unsigned char)orig);
        if (busca_solucao(t)) 
            return true;
        // desfaz
        t->tabuleiro[bi][bj] = orig;
    }

    // 5) Tenta riscar (‘#’)
    {
        char orig = t->tabuleiro[bi][bj];
        t->tabuleiro[bi][bj] = '#';

        // só prossegue se não violar já as regras locais
        if (!violacao_basica(t)) {
            if (busca_solucao(t)) 
                return true;
        }

        // desfaz
        t->tabuleiro[bi][bj] = orig;
    }

    // 6) Nenhuma das duas opções funcionou:
    return false;
}
void comando_R(Tabuleiro *t) {
    // Cria backup do estado atual
    char** backup = malloc(t->linhas * sizeof(char*));
    if (!backup) {
        perror("Erro ao alocar backup");
        return;
    }
    
    for (int i = 0; i < t->linhas; i++) {
        backup[i] = malloc(t->colunas * sizeof(char));
        if (!backup[i]) {
            perror("Erro ao alocar linha do backup");
            // Liberta o que já foi alocado
            for (int j = 0; j < i; j++) free(backup[j]);
            free(backup);
            return;
        }
        memcpy(backup[i], t->tabuleiro[i], t->colunas);
    }

    // Tenta resolver
    if (resolver_recursivo(t)) {
        printf("Solução encontrada:\n");
        // Usa a função de impressão com a assinatura especificada
        imprimirTabuleiro(t->tabuleiro, t->linhas, t->colunas);
    } else {
        printf("Não foi possível resolver o tabuleiro.\n");
        // Restaura o estado original
        for (int i = 0; i < t->linhas; i++) {
            memcpy(t->tabuleiro[i], backup[i], t->colunas);
        }
    }

    // Liberta o backup em ambos os casos
    for (int i = 0; i < t->linhas; i++) {
        free(backup[i]);
    }
    free(backup);
}


// Função recursiva de backtracking (modificação in-place)
bool resolver_recursivo(Tabuleiro* t) {
    // Encontra próxima célula não decidida (minúscula)
    int linha = -1, coluna = -1;
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (islower(t->tabuleiro[i][j])) {
                linha = i;
                coluna = j;
                break;
            }
        }
        if (linha != -1) break;
    }

    // Todas células decididas - verifica solução completa
    if (linha == -1) {
        return verificar_conectividade(t);
    }

    char original = t->tabuleiro[linha][coluna];
    
    // Tenta marcar como branco
    t->tabuleiro[linha][coluna] = toupper(original);
    if (validacao_parcial(t, linha, coluna)) {
        if (resolver_recursivo(t)) return true;
    }
    
    // Tenta marcar como riscado
    t->tabuleiro[linha][coluna] = '#';
    if (validacao_parcial(t, linha, coluna)) {
        if (resolver_recursivo(t)) return true;
    }
    
    // Backtrack: reverte para estado original
    t->tabuleiro[linha][coluna] = original;
    return false;
}

// Função auxiliar: verificação de restrições locais
bool validacao_parcial(Tabuleiro* t, int linha, int coluna) {
    char c = t->tabuleiro[linha][coluna];
    
    // Se for branco, verifica duplicatas na linha/coluna
    if (isupper(c)) {
        // Verifica linha
        for (int j = 0; j < t->colunas; j++) {
            if (j != coluna && t->tabuleiro[linha][j] == c) return false;
        }
        // Verifica coluna
        for (int i = 0; i < t->linhas; i++) {
            if (i != linha && t->tabuleiro[i][coluna] == c) return false;
        }
    } 
    // Se for riscado, verifica vizinhos
    else if (c == '#') {
        int viz[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for (int v = 0; v < 4; v++) {
            int ni = linha + viz[v][0];
            int nj = coluna + viz[v][1];
            if (ni >= 0 && ni < t->linhas && nj >= 0 && nj < t->colunas) {
                if (t->tabuleiro[ni][nj] == '#') return false; // Dois riscados adjacentes
            }
        }
    }
    return true;
}

// Função auxiliar: verificação de conectividade (DFS)
void dfs_conectividade(Tabuleiro* t, int i, int j, bool** visitado) {
    if (i < 0 || i >= t->linhas || j < 0 || j >= t->colunas) return;
    if (visitado[i][j] || t->tabuleiro[i][j] == '#') return;
    
    visitado[i][j] = true;
    dfs_conectividade(t, i-1, j, visitado); // Cima
    dfs_conectividade(t, i+1, j, visitado); // Baixo
    dfs_conectividade(t, i, j-1, visitado); // Esquerda
    dfs_conectividade(t, i, j+1, visitado); // Direita
}

// Função principal de verificação de conectividade
bool verificar_conectividade(Tabuleiro* t) {
    // Aloca matriz de visitados
    bool** visitado = malloc(t->linhas * sizeof(bool*));
    for (int i = 0; i < t->linhas; i++) {
        visitado[i] = calloc(t->colunas, sizeof(bool));
    }

    // Encontra primeira célula branca para iniciar DFS
    bool encontrou_inicio = false;
    int start_i = -1, start_j = -1;
    for (int i = 0; i < t->linhas && !encontrou_inicio; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (isupper(t->tabuleiro[i][j])) {
                start_i = i;
                start_j = j;
                encontrou_inicio = true;
                break;
            }
        }
    }

    // Se não há células brancas, considera inválido
    if (!encontrou_inicio) {
        for (int i = 0; i < t->linhas; i++) free(visitado[i]);
        free(visitado);
        return false;
    }

    // Executa DFS a partir da primeira célula branca
    dfs_conectividade(t, start_i, start_j, visitado);

    // Verifica se todas as brancas foram visitadas
    bool conectado = true;
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (isupper(t->tabuleiro[i][j]) && !visitado[i][j]) {
                conectado = false;
                break;
            }
        }
    }

    // Liberta memória
    for (int i = 0; i < t->linhas; i++) free(visitado[i]);
    free(visitado);

    return conectado;
}

// Funções de criação e libertação do tabuleiro
Tabuleiro criar_tabuleiro(int l, int c) {
    Tabuleiro t;
    t.linhas = l;
    t.colunas = c;
    t.tabuleiro = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++) {
        t.tabuleiro[i] = malloc(c * sizeof(char));
    }
    return t;
}

// Liberta toda a memória de um Tabuleiro
void libertar_tabuleiro(Tabuleiro* t) {
    if (!t->tabuleiro) return;
    for (int i = 0; i < t->linhas; i++) free(t->tabuleiro[i]);
    free(t->tabuleiro);
    t->tabuleiro = NULL;
}

Tabuleiro copia_tabuleiro(Tabuleiro *t) {
    Tabuleiro c;
    c.linhas = t->linhas;
    c.colunas = t->colunas;
    c.tabuleiro = malloc(c.linhas * sizeof(char *));
    for (int i = 0; i < c.linhas; i++) {
        c.tabuleiro[i] = malloc(c.colunas * sizeof(char));
        memcpy(c.tabuleiro[i], t->tabuleiro[i], c.colunas * sizeof(char));
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


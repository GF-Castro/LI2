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
        if (tabuleiro[y][x] == '#') {
            printf("Não é possível pintar de branco uma casa já riscada.\n");
            return;
        }
        char ant = tabuleiro[y][x];
        char prox = toupper(tabuleiro[y][x]);
        guardar_move('b', x, y, ant, prox);
        tabuleiro[y][x] = prox;
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void riscar(char **tabuleiro, int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        char ant = tabuleiro[y][x];
        char prox = '#';
        guardar_move('r', x, y, ant, prox);
        tabuleiro[y][x] = prox;
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void guardar_move(char ação, int x, int y, char ant_val, char novo_val) {
    if (topoStack < tamanhoStack - 1) {
        topoStack++;
        movestack[topoStack].ação = ação;
        movestack[topoStack].x = x;
        movestack[topoStack].y = y;
        movestack[topoStack].ant_val = ant_val;
        movestack[topoStack].novo_val = novo_val;
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
    t->tabuleiro[m.y][m.x] = m.ant_val;
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
        fprintf(f, "%c %d %d %c %c\n", m->ação, m->x, m->y, m->ant_val, m->novo_val);
    }
    fclose(f);
}

// Função que grava o tabuleiro + stack num ficheiro específico
void gravarJogo(char *nomeTabuleiro, Tabuleiro *t, char *nomeStack) {
    // Grava o tabuleiro em "nomeTabuleiro"
    FILE *arquivo = fopen(nomeTabuleiro, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir ficheiro do tabuleiro para gravar");
        return;
    }

    // Grava dimensões: linhas colunas
    fprintf(arquivo, "%d %d\n", t->linhas, t->colunas);

    // Grava cada linha do tabuleiro
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            fputc(t->tabuleiro[i][j], arquivo);
        }
        fputc('\n', arquivo);
    }
    fclose(arquivo);

    // Agora grava a stack de movimentos em "nomeStack"
    gravarStack(nomeStack);
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

    topoStack = -1;

    for (int i = 0; i < quantidade; i++) {
        char ação, ant_val, novo_val;
        int x, y;
        if (fscanf(f, " %c %d %d %c %c\n", &ação, &x, &y, &ant_val, &novo_val) == 5) {
            movestack[i].ação = ação;
            movestack[i].x = x;
            movestack[i].y = y;
            movestack[i].ant_val = ant_val;
            movestack[i].novo_val = novo_val;
            topoStack = i;
        } else {
            i = quantidade; // força a saída
        }
    }

    fclose(f);
}


void consumirNovaLinha(FILE *f) {
    int ch;
    while ((ch = fgetc(f)) != EOF && ch != '\n');
}

void lerJogo(char *nome, Tabuleiro *t) {
    t->linhas = 0;
    t->colunas = 0;
    t->tabuleiro = NULL;
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

void aplicar_correcoes(Tabuleiro *t) {
    // 1) Faz uma cópia “orig” do estado atual do tabuleiro
    Tabuleiro orig = *t;

    int alteracoes = 0;

    // 2) Prepara um buffer “novo” para receber as alterações
    //    (alocamos matriz temporária no mesmo tamanho de orig)
    char **novo = malloc(orig.linhas * sizeof(char *));
    if (!novo) { perror("malloc novo linhas"); exit(1); }
    for (int i = 0; i < orig.linhas; i++) {
        novo[i] = malloc(orig.colunas * sizeof(char));
        if (!novo[i]) { perror("malloc novo colunas"); exit(1); }
        // Copia toda a linha de orig para novo
        memcpy(novo[i], orig.tabuleiro[i], orig.colunas * sizeof(char));
    }

    // 3) REGRA 1: Para cada branca (maiúscula) em orig, riscar as suas réplicas minúsculas em novo
    for (int i = 0; i < orig.linhas; i++) {
        for (int j = 0; j < orig.colunas; j++) {
            char c = orig.tabuleiro[i][j];
            if (isupper( c)) {
                char minus = tolower( c);
                // nas colunas da linha i
                for (int col = 0; col < orig.colunas; col++) {
                    if (orig.tabuleiro[i][col] == minus && novo[i][col] != '#') {
                        novo[i][col] = '#';
                    }
                }
                // nas linhas da coluna j
                for (int lin = 0; lin < orig.linhas; lin++) {
                    if (orig.tabuleiro[lin][j] == minus && novo[lin][j] != '#') {
                        novo[lin][j] = '#';
                    }
                }
            }
        }
    }

    // 4) REGRA 2: Para cada “#” em orig, pintar de branco os vizinhos minúsculos em novo
    int di[4] = {-1, 1, 0, 0}, dj[4] = {0, 0, -1, 1};
    for (int i = 0; i < orig.linhas; i++) {
        for (int j = 0; j < orig.colunas; j++) {
            if (orig.tabuleiro[i][j] == '#') {
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d], nj = j + dj[d];
                    if (ni >= 0 && ni < orig.linhas && nj >= 0 && nj < orig.colunas) {
                        char viz = orig.tabuleiro[ni][nj];
                        if (islower(viz)) {
                            char mai = toupper(viz);
                            if (novo[ni][nj] != mai) {
                                novo[ni][nj] = mai;
                            }
                        }
                    }
                }
            }
        }
    }

    // 5) Agora percorre “novo” e empilha (guardar_move) qualquer célula que tenha mudado em t->tabuleiro
    for (int i = 0; i < orig.linhas; i++) {
        for (int j = 0; j < orig.colunas; j++) {
            char antes = orig.tabuleiro[i][j];
            char depois = novo[i][j];
            if (antes != depois) {
                // Empilha o movimento para permitir desfazer:
                // ação = 'a', coordenadas (j=coluna, i=linha), ant_val=antes, novo_val=depois
                guardar_move('a', j, i, antes, depois);
                alteracoes++;
            }
            // Atualiza o tabuleiro “real” com o novo valor
            t->tabuleiro[i][j] = depois;
        }
    }

    // 6) Limpa o buffer “novo”
    for (int i = 0; i < orig.linhas; i++) {
        free(novo[i]);
    }
    free(novo);

    printf("Alterações aplicadas: %d\n", alteracoes);
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


bool resolver_recursivo(Tabuleiro *t) {
    if (t == NULL || t->tabuleiro == NULL || t->linhas <= 0 || t->colunas <= 0)
        return false;
    // Encontra próxima célula não decidida (minúscula)
    int linha = -1, coluna = -1;
    int encontrou = 0;
    for (int i = 0; i < t->linhas && !encontrou; i++) {
        for (int j = 0; j < t->colunas && !encontrou; j++) {
            if (islower(t->tabuleiro[i][j])) {
                linha = i;
                coluna = j;
                encontrou = 1;
            }
        }
    }

    // Todas as células decididas - verifica solução completa
    if (linha == -1 && coluna == -1) {
        return verificar_conectividade(t);
    }

    char original = t->tabuleiro[linha][coluna];
    bool resultado = false;

    // Tenta marcar como branco
    t->tabuleiro[linha][coluna] = toupper(original);
    if (validacao_parcial(t, linha, coluna)) {
        resultado = resolver_recursivo(t);
    }

    // Se não deu certo, tenta marcar como riscado
    if (!resultado) {
        t->tabuleiro[linha][coluna] = '#';
        if (validacao_parcial(t, linha, coluna)) {
            resultado = resolver_recursivo(t);
        }
    }

    // Backtrack: reverte para estado original
    if (!resultado) {
        t->tabuleiro[linha][coluna] = original;
    }

    return resultado;
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

bool verificar_conectividade(Tabuleiro* t) {
    // Aloca matriz de visitados
    bool** visitado = malloc(t->linhas * sizeof(bool*));
    for (int i = 0; i < t->linhas; i++) {
        visitado[i] = calloc(t->colunas, sizeof(bool));
    }

    // Encontra primeira célula branca para iniciar DFS 
    bool encontrou_inicio = false;
    int start_i = -1, start_j = -1;
    bool sair_for_interno = false;

    for (int i = 0; i < t->linhas && !encontrou_inicio; i++) {
        sair_for_interno = false;
        for (int j = 0; j < t->colunas && !sair_for_interno; j++) {
            if (isupper(t->tabuleiro[i][j])) {
                start_i = i;
                start_j = j;
                encontrou_inicio = true;
                sair_for_interno = true;
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
    for (int i = 0; i < t->linhas && conectado; i++) {
        for (int j = 0; j < t->colunas && conectado; j++) {
            if (isupper(t->tabuleiro[i][j]) && !visitado[i][j]) {
                conectado = false;
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

Tabuleiro copiar_tabuleiro(Tabuleiro *t) {
    Tabuleiro novo;
    novo.linhas = t->linhas;
    novo.colunas = t->colunas;

    novo.tabuleiro = malloc(novo.linhas * sizeof(char*));
    for (int i = 0; i < novo.linhas; i++) {
        novo.tabuleiro[i] = malloc(novo.colunas * sizeof(char));
        memcpy(novo.tabuleiro[i], t->tabuleiro[i], novo.colunas);
    }

    return novo;
}

// Funções de interface
void imprimir_comandos() {
    printf("Comandos disponíveis:\n");
    printf("g - Gravar o jogo atual no ficheiro\n");
    printf("l - Ler o estado do jogo num ficheiro\n");
    printf("b letra numero - Pintar de branco\n");
    printf("r letra numero - Riscar\n");
    printf("v - Verificar restrições\n");
    printf("a - Aplicar inferências\n");
    printf("A - Repetir inferências até não haver mudanças\n");
    printf("R - Resolver o jogo\n");
    printf("d - Desfazer última ação\n");
    printf("s - Sair\n");
}

// Retorna true se existir alguma célula com letra minúscula
bool tem_minusculas(Tabuleiro *t) {
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (islower( t->tabuleiro[i][j])) {
                return true;
            }
        }
    }
    return false;
}

// Função auxiliar para comparar dois tabuleiros (deep compare)
bool tabuleiros_iguais(Tabuleiro *a, Tabuleiro *b) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) return false;
    for (int i = 0; i < a->linhas; i++)
        for (int j = 0; j < a->colunas; j++)
            if (a->tabuleiro[i][j] != b->tabuleiro[i][j])
                return false;
    return true;
}
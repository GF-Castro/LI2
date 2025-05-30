/*
 * solver_improvements.c
 * Implementação de heurísticas e estruturas para acelerar o solver:
 * 1. Bit-masks para linhas/colunas
 * 2. Union-Find reversível (undo)
 * 3. Forward-checking (propagação de regras)
 * 4. MRV (Minimum Remaining Values)
 * 5. Busca recursiva usando undo sem mallocs
 */

 #include <stdlib.h>
 #include <stdio.h>
 #include <stdbool.h>
 #include <ctype.h>
 #include <string.h>
 #include "projeto.h"  // assume Tabuleiro e libertar/criar definidos
 
 // ----------------------
 // 1. BIT-MASKS
 // ----------------------
 // Um bit por letra A-Z
 static unsigned int *linhaMask;
 static unsigned int *colunaMask;
 
 // Inicializa masks (chamar após criar_tabuleiro)
 void init_masks(int linhas, int colunas) {
     linhaMask = calloc(linhas, sizeof(unsigned int));
     colunaMask = calloc(colunas, sizeof(unsigned int));
     if (!linhaMask || !colunaMask) { perror("calloc masks"); exit(1); }
 }
 
 // Liberta masks
 void free_masks() {
     free(linhaMask);
     free(colunaMask);
 }
 
 // Tenta pintar branca letra k em (i,j): retorna false se conflito
 bool set_branca(int i, int j, int k) {
     unsigned int bit = 1u << k;
     if (linhaMask[i] & bit) return false;
     if (colunaMask[j] & bit) return false;
     linhaMask[i] |= bit;
     colunaMask[j] |= bit;
     return true;
 }
 
 // Desfaz pintar branca
 void unset_branca(int i, int j, int k) {
     unsigned int bit = 1u << k;
     linhaMask[i] &= ~bit;
     colunaMask[j] &= ~bit;
 }
 
 // ----------------------
 // 2. UNION-FIND REVERSÍVEL
 // ----------------------
 
 typedef struct UFOp { int x, parent_x, rank_x; int y, parent_y, rank_y; } UFOp;
 
 static int *parent, *uf_rank, uf_count;
 static UFOp *uf_op_stack;
 static int uf_op_top;
 
 void uf_init(int n) {
     parent = malloc(n * sizeof(int));
     uf_rank = calloc(n, sizeof(int));
     uf_op_stack = malloc(n * sizeof(UFOp));
     uf_op_top = 0;
     uf_count = n;
     if (!parent || !uf_rank || !uf_op_stack) { perror("uf malloc"); exit(1); }
     for (int i = 0; i < n; i++) parent[i] = i;
 }
 
 int uf_find(int x) {
     while (parent[x] != x) x = parent[x];
     return x;
 }
 
 void uf_union(int x, int y) {
     int rx = uf_find(x), ry = uf_find(y);
     if (rx == ry) return;
     // guarda operação para undo
     uf_op_stack[uf_op_top++] = (UFOp){rx, parent[rx], uf_rank[rx], ry, parent[ry], uf_rank[ry]};
     uf_count--;
     // union by rank
     if (uf_rank[rx] < uf_rank[ry]) {
         parent[rx] = ry;
     } else if (uf_rank[rx] > uf_rank[ry]) {
         parent[ry] = rx;
     } else {
         parent[ry] = rx;
         uf_rank[rx]++;
     }
 }
 
 void uf_undo_to(int top) {
     while (uf_op_top > top) {
         UFOp op = uf_op_stack[--uf_op_top];
         parent[op.x] = op.parent_x;
         uf_rank[op.x] = op.rank_x;
         parent[op.y] = op.parent_y;
         uf_rank[op.y] = op.rank_y;
         uf_count++;
     }
 }
 
 void uf_free() {
     free(parent);
     free(uf_rank);
     free(uf_op_stack);
 }
 
 // ----------------------
 // 3. FORWARD-CHECKING
 // ----------------------
 
 // aplica regras de propagação até fixpoint; retorna false se falhar
 bool forward_check(Tabuleiro *t) {
     bool changed;
     do {
         changed = false;
         for (int i = 0; i < t->linhas; i++) {
             for (int j = 0; j < t->colunas; j++) {
                 char c = t->tabuleiro[i][j];
                 if (islower(c)) continue;
                 if (c == '#') {
                     // todos vizinhos ortogonais devem ser maiúscula
                     int di[4] = {-1,1,0,0}, dj[4] = {0,0,-1,1};
                     for (int d = 0; d < 4; d++) {
                         int ni = i+di[d], nj = j+dj[d];
                         if (ni>=0&&ni<t->linhas&&nj>=0&&nj<t->colunas) {
                             if (islower(t->tabuleiro[ni][nj])) {
                                 // pintar vizinho de branco
                                 int k = toupper(t->tabuleiro[ni][nj]) - 'A';
                                 if (!set_branca(ni,nj,k)) return false;
                                 t->tabuleiro[ni][nj] = toupper(t->tabuleiro[ni][nj]);
                                 changed = true;
                             }
                         }
                     }
                 } else if (isupper(c)) {
                     int k = c - 'A';
                     // todas réplicas minúsculas dessa letra na linha e coluna devem virar riscado
                     for (int x = 0; x < t->colunas; x++) {
                         if (t->tabuleiro[i][x] == tolower(c)) {
                             t->tabuleiro[i][x] = '#';
                             changed = true;
                         }
                     }
                     for (int y = 0; y < t->linhas; y++) {
                         if (t->tabuleiro[y][j] == tolower(c)) {
                             t->tabuleiro[y][j] = '#';
                             changed = true;
                         }
                     }
                 }
             }
         }
     } while (changed);
     return true;
 }
 
 // ----------------------
 // 4. MRV (Minimum Remaining Values)
 // ----------------------
 
 // calcula número de opções (0,1 ou 2) para célula (i,j)
 int count_options(Tabuleiro *t, int i, int j) {
     int opts = 0;
     char c = t->tabuleiro[i][j];
     int k = tolower(c) - 'a';
     // opcão branco
     if (set_branca(i,j,k)) {
         unset_branca(i,j,k);
         opts++;
     }
     // opção riscar: verificar vizinhos
     bool ok = true;
     int di[4] = {-1,1,0,0}, dj[4] = {0,0,-1,1};
     for (int d = 0; d < 4; d++) {
         int ni = i+di[d], nj = j+dj[d];
         if (ni>=0&&ni<t->linhas&&nj>=0&&nj<t->colunas) {
             if (t->tabuleiro[ni][nj] == '#') { ok=false; break; }
         }
     }
     if (ok) opts++;
     return opts;
 }
 
 // seleciona próxima célula pelo MRV; retorna false se nenhuma resta
 bool select_MRV(Tabuleiro *t, int *outi, int *outj) {
     int best = 3;
     int bi=-1, bj=-1;
     for (int i = 0; i < t->linhas; i++) {
         for (int j = 0; j < t->colunas; j++) {
             if (islower(t->tabuleiro[i][j])) {
                 int opts = count_options(t,i,j);
                 if (opts < best) {
                     best = opts;
                     bi = i; bj = j;
                     if (best <= 1) break;
                 }
             }
         }
         if (best <= 1) break;
     }
     if (bi<0) return false;
     *outi = bi; *outj = bj;
     return true;
 }
 
 // ----------------------
 // 5. BUSCA RECURSIVA COM UNDO
 // ----------------------
 
 bool busca_avancada(Tabuleiro *t);
 
 bool busca_avancada(Tabuleiro *t) {
     if (!forward_check(t)) return false;
     if (completo(t)) return uf_count==1;
 
     int i,j;
     if (!select_MRV(t,&i,&j)) return false;
 
     char orig = t->tabuleiro[i][j];
     int k = tolower(orig)-'a';
 
     // guarda estado UF
     int uf_top = uf_op_top;
 
     // tenta branco
     if (set_branca(i,j,k)) {
         t->tabuleiro[i][j] = toupper(orig);
         // liga UF com vizinhos brancos
         int di[4]={-1,1,0,0}, dj[4]={0,0,-1,1};
         int idx = i*t->colunas+j;
         for (int d=0;d<4;d++) {
             int ni=i+di[d], nj=j+dj[d];
             if (ni>=0&&ni<t->linhas&&nj>=0&&nj<t->colunas && isupper(t->tabuleiro[ni][nj])) {
                 uf_union(idx, ni*t->colunas+nj);
             }
         }
         if (busca_avancada(t)) return true;
         // undo UF
         uf_undo_to(uf_top);
         unset_branca(i,j,k);
         t->tabuleiro[i][j] = orig;
     }
 
     // tenta riscar
     bool can_r = true;
     int di2[4]={-1,1,0,0}, dj2[4]={0,0,-1,1};
     for (int d=0; d<4; d++) {
         int ni=i+di2[d], nj=j+dj2[d];
         if (ni>=0&&ni<t->linhas&&nj>=0&&nj<t->colunas && t->tabuleiro[ni][nj]=='#') { can_r=false; break; }
     }
     if (can_r) {
         t->tabuleiro[i][j] = '#';
         if (busca_avancada(t)) return true;
         t->tabuleiro[i][j] = orig;
     }
 
     return false;
 }
 
 // wrapper:
 bool busca_solucao(Tabuleiro *t) {
     init_masks(t->linhas, t->colunas);
     uf_init(t->linhas * t->colunas);
 
     // inicializa UF e masks com estados brancos existentes
     for (int i=0;i<t->linhas;i++) for (int j=0;j<t->colunas;j++) {
         char c = t->tabuleiro[i][j];
         if (isupper(c)) {
             int k = c-'A'; set_branca(i,j,k);
         }
     }
     // UF liga brancos iniciais
     for (int i=0;i<t->linhas;i++) for (int j=0;j<t->colunas;j++) {
         if (isupper(t->tabuleiro[i][j])) {
             int idx = i*t->colunas+j;
             int di[4]={-1,1,0,0}, dj[4]={0,0,-1,1};
             for (int d=0;d<4;d++) {
                 int ni=i+di[d], nj=j+dj[d];
                 if (ni>=0&&ni<t->linhas&&nj>=0&&nj<t->colunas && isupper(t->tabuleiro[ni][nj])) {
                     uf_union(idx, ni*t->colunas+nj);
                 }
             }
         }
     }
 
     bool ok = busca_avancada(t);
 
     uf_free();
     free_masks();
     return ok;
 }
 
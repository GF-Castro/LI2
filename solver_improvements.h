#ifndef SOLVER_IMPROVEMENTS_H
#define SOLVER_IMPROVEMENTS_H

#include "projeto.h"

// Inicialização e libertação de estruturas internas
void init_masks(int linhas, int colunas);
void free_masks(void);

// Solver principal otimizado
bool busca_solucao(Tabuleiro *t);

#endif // SOLVER_IMPROVEMENTS_H

#include "projeto.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

// Helper function to create a test board
Tabuleiro criar_tabuleiro_teste(int linhas, int colunas, const char *content) {
    Tabuleiro t = criar_tabuleiro(linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            t.tabuleiro[i][j] = content[i * colunas + j];
        }
    }
    return t;
}

// Teste para a função lerJogo
void teste_lerJogo() {
    Tabuleiro t = {0};
    
    // Caso de teste: ficheiro com dimensões e tabuleiro válidos
    FILE *f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "3 3\nABC\nDEF\nGHI\n");
    fclose(f);
    
    lerJogo("test_tabuleiro.txt", &t);
    CU_ASSERT_EQUAL(t.linhas, 3);
    CU_ASSERT_EQUAL(t.colunas, 3);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'E');
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], 'I');
    libertar_tabuleiro(&t);
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro vazio
    f = fopen("test_tabuleiro.txt", "w");
    fclose(f);
    lerJogo("test_tabuleiro.txt", &t);
    libertar_tabuleiro(&t);
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro inexistente
    lerJogo("nao_existe.txt", &t);
}

// Teste para a função pintarDeBranco
void teste_pintarDeBranco() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "abcdefghi");

    // Caso de teste: dentro dos limites
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 1, 1);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'E');

    // Caso de teste: fora dos limites (negativos)
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, -1, -1);

    // Caso de teste: fora dos limites (maiores que o tamanho do tabuleiro)
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 100, 100);

    // Caso de teste: célula já em maiúscula
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 0, 0);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], 'A');

    libertar_tabuleiro(&t);
}

// Teste para a função riscar
void teste_riscar() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "abcdefghi");

    // Caso de teste: dentro dos limites
    riscar(t.tabuleiro, t.linhas, t.colunas, 2, 2);
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], '#');

    // Caso de teste: fora dos limites (negativos)
    riscar(t.tabuleiro, t.linhas, t.colunas, -1, -1);

    // Caso de teste: fora dos limites (maiores que o tamanho do tabuleiro)
    riscar(t.tabuleiro, t.linhas, t.colunas, 100, 100);

    // Caso de teste: célula já riscada
    riscar(t.tabuleiro, t.linhas, t.colunas, 2, 2);
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], '#');

    libertar_tabuleiro(&t);
}

// Teste para a função formatoParaCoordenadas
void teste_formatoParaCoordenadas() {
    int x, y;

    // Caso de teste: formato válido
    CU_ASSERT_TRUE(formatoParaCoordenadas("b2", &x, &y));
    CU_ASSERT_EQUAL(x, 1);
    CU_ASSERT_EQUAL(y, 1);

    // Caso de teste: formato inválido
    CU_ASSERT_FALSE(formatoParaCoordenadas("invalido", &x, &y));
    CU_ASSERT_FALSE(formatoParaCoordenadas("123", &x, &y));
    CU_ASSERT_FALSE(formatoParaCoordenadas("z!", &x, &y));

    // Caso de teste: formato vazio
    CU_ASSERT_FALSE(formatoParaCoordenadas("", &x, &y));

    // Caso de teste: caracteres especiais
    CU_ASSERT_FALSE(formatoParaCoordenadas("!@#", &x, &y));
}

void teste_formatoParaCoordenadas_extendido() {
    int x, y;
    
    // Teste limites do tabuleiro
    CU_ASSERT_TRUE(formatoParaCoordenadas("a1", &x, &y));
    CU_ASSERT_EQUAL(x, 0);
    CU_ASSERT_EQUAL(y, 0);
    
    // Teste com letra maiúscula
    CU_ASSERT_TRUE(formatoParaCoordenadas("B3", &x, &y));
    CU_ASSERT_EQUAL(x, 1);
    CU_ASSERT_EQUAL(y, 2);
    
    // Teste com números maiores
    CU_ASSERT_TRUE(formatoParaCoordenadas("z99", &x, &y));
    CU_ASSERT_EQUAL(x, 25);
    CU_ASSERT_EQUAL(y, 98);
    
    // Teste com strings mal formatadas
    CU_ASSERT_FALSE(formatoParaCoordenadas("a", &x, &y));
    CU_ASSERT_FALSE(formatoParaCoordenadas("1a", &x, &y));
    CU_ASSERT_FALSE(formatoParaCoordenadas("a1b", &x, &y));
}

void teste_imprimirTabuleiro() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "ABCDEFGHI");
    imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
    libertar_tabuleiro(&t);
}

void teste_imprimir_comandos() {
    imprimir_comandos();
}

void teste_gerenciamento_estado() {
    topoStack = -1;
    memset(movestack, 0, sizeof(movestack));

    Tabuleiro t1 = criar_tabuleiro_teste(2, 2, "abcd");
    Tabuleiro t2 = criar_tabuleiro_teste(2, 2, "efgh");
    
    // Teste empilhar e desempilhar
    guardar_move('b', 0, 0, 'a', 'A');
    CU_ASSERT_EQUAL(topoStack, 0);
    
    guardar_move('r', 1, 1, 'h', '#');
    CU_ASSERT_EQUAL(topoStack, 1);
    
    Move m = desempilhar();
    CU_ASSERT_EQUAL(m.ação, 'r');
    CU_ASSERT_EQUAL(topoStack, 0);
    
    m = desempilhar();
    CU_ASSERT_EQUAL(m.ação, 'b');
    CU_ASSERT_EQUAL(topoStack, -1);
    
    // Teste desempilhar com stack vazia
    m = desempilhar();
    CU_ASSERT_EQUAL(topoStack, -1);
    
    libertar_tabuleiro(&t1);
    libertar_tabuleiro(&t2);
}

void teste_verificacao_regras() {
    Tabuleiro t_valido = criar_tabuleiro_teste(3, 3, "A#B#C#D#E");
    Tabuleiro t_invalido = criar_tabuleiro_teste(3, 3, "A#A#B#C#B");
    
    verificar_riscadas(&t_valido);
    verificar_brancas(&t_valido);
    verificar_riscadas(&t_invalido);
    verificar_brancas(&t_invalido);
    verificar_estado(&t_valido);
    verificar_estado(&t_invalido);
    
    libertar_tabuleiro(&t_valido);
    libertar_tabuleiro(&t_invalido);
}


void teste_arquivos() {
    topoStack = -1;
    memset(movestack, 0, sizeof(movestack));

    Tabuleiro t_original = criar_tabuleiro_teste(2, 2, "abcd");
    Tabuleiro t_lido = {0};
    
    gravarJogo("teste_jogo.txt", &t_original, NULL);
    lerJogo("teste_jogo.txt", &t_lido);
    CU_ASSERT_EQUAL(t_lido.linhas, 2);
    CU_ASSERT_EQUAL(t_lido.colunas, 2);
    CU_ASSERT_EQUAL(t_lido.tabuleiro[0][0], 'a');
    libertar_tabuleiro(&t_original);
    libertar_tabuleiro(&t_lido);
    remove("teste_jogo.txt");
    
    // Teste stack
    guardar_move('b', 0, 0, 'a', 'A');
    gravarStack("teste_stack.txt");
    topoStack = -1;
    lerStack("teste_stack.txt");
    CU_ASSERT_EQUAL(topoStack, 0);
    CU_ASSERT_EQUAL(movestack[0].ação, 'b');
    remove("teste_stack.txt");
    
    // Teste com arquivos inexistentes
    lerJogo("nao_existe.txt", &t_lido);
    lerStack("nao_existe.txt");
}

void teste_stacks_cheia() {
    topoStack = -1;
    for (int i = 0; i < tamanhoStack; i++) {
        guardar_move('b', 0, 0, 'a', 'A');
    }
    CU_ASSERT_EQUAL(topoStack, tamanhoStack - 1);
    guardar_move('b', 0, 0, 'a', 'A');
    CU_ASSERT_EQUAL(topoStack, tamanhoStack - 1);
    topoStack = -1;
}

void teste_desfazer_vazio() {
    topoStack = -1;
    Tabuleiro t = criar_tabuleiro_teste(2, 2, "ABCD");
    desfazer(&t);
    CU_ASSERT_EQUAL(topoStack, -1);
    libertar_tabuleiro(&t);
}

void teste_gravarStack_erro_abertura() {
    mkdir("tmpdir", 0700);
    gravarStack("tmpdir");
    rmdir("tmpdir");
    CU_PASS("gravarStack erro de abertura coberto");
}

void teste_gravarJogo_erro_abertura() {
    mkdir("tmpdir2", 0700);
    Tabuleiro t = criar_tabuleiro_teste(1, 1, "B");
    gravarJogo("tmpdir2", &t, NULL);
    libertar_tabuleiro(&t);
    rmdir("tmpdir2");
    CU_PASS("gravarJogo erro de abertura coberto");
}

void teste_lerStack_header_malformado() {
    FILE *f = fopen("bad1.txt", "w");
    fprintf(f, "xyz\n");
    fclose(f);
    lerStack("bad1.txt");
    CU_ASSERT_EQUAL(topoStack, -1);
    remove("bad1.txt");
}

void teste_lerStack_linha_incompleta() {
    FILE *f = fopen("bad2.txt", "w");
    fprintf(f, "1\n");
    fprintf(f, "3\n");
    fclose(f);
    lerStack("bad2.txt");
    CU_ASSERT_EQUAL(topoStack, -1);
    remove("bad2.txt");
}

void teste_verificar_riscadas_borda() {
    Tabuleiro t = criar_tabuleiro_teste(2, 2, "#A#B");
    verificar_riscadas(&t);
    CU_PASS("verificar_riscadas borda coberto");
    libertar_tabuleiro(&t);
}

void teste_verificar_riscadas_interior() {
    Tabuleiro t = criar_tabuleiro(3, 3);
    t.tabuleiro[1][1] = '#';
    verificar_riscadas(&t);
    CU_PASS("verificar_riscadas interior coberto");
    libertar_tabuleiro(&t);
}

void teste_verificar_brancas_coluna() {
    Tabuleiro t = criar_tabuleiro_teste(3, 1, "CCC");
    verificar_brancas(&t);
    CU_PASS("verificar_brancas coluna coberto");
    libertar_tabuleiro(&t);
}

void teste_verificar_brancas_coluna_dupla() {
    Tabuleiro t = criar_tabuleiro_teste(2, 2, "ADBD");
    verificar_brancas(&t);
    CU_PASS("verificar_brancas coluna dupla coberto");
    libertar_tabuleiro(&t);
}

void teste_lerJogo_header_malformado() {
    FILE *f = fopen("ljbad.txt", "w");
    fprintf(f, "foo bar\n");
    fclose(f);
    Tabuleiro t = {0};
    lerJogo("ljbad.txt", &t);
    remove("ljbad.txt");
    CU_PASS("lerJogo header malformado coberto");
}

void teste_aplicar_correcoes() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "aAB#cDefG");
    aplicar_correcoes(&t);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'C');
    CU_ASSERT_EQUAL(t.tabuleiro[2][0], 'E');
    libertar_tabuleiro(&t);
}

void teste_resolve_jogo() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "abcdefghi");
    comando_R(&t);
    int mudou = (isupper(t.tabuleiro[0][0]) || t.tabuleiro[0][0] == '#');
    CU_ASSERT_TRUE(mudou);
    libertar_tabuleiro(&t);
}

void teste_verificar_conectividade_isoladas() {
    Tabuleiro t = criar_tabuleiro(3, 3);
    t.tabuleiro[0][0] = 'A';
    t.tabuleiro[2][2] = 'B';
    verificar_conectividade(&t);
    CU_PASS("verificar_conectividade isoladas coberto");
    libertar_tabuleiro(&t);
}

void teste_formatoParaCoordenadas_NULL() {
    int x, y;
    CU_ASSERT_FALSE(formatoParaCoordenadas(NULL, &x, &y));
}

void teste_pintar_vizinhos_de_branco() {
    Tabuleiro t = criar_tabuleiro(3, 3);
    t.tabuleiro[1][1] = '#';
    t.tabuleiro[0][1] = 'a';
    t.tabuleiro[1][0] = 'b';
    
    int mudou = pintar_vizinhos_de_branco(&t, 1, 1);
    CU_ASSERT_EQUAL(mudou, 1);
    CU_ASSERT_EQUAL(t.tabuleiro[0][1], 'A');
    CU_ASSERT_EQUAL(t.tabuleiro[1][0], 'B');
    libertar_tabuleiro(&t);
}

void teste_risco_isolamento() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "A#######B");
    t.tabuleiro[1][1] = 'c';
    aplicar_correcoes(&t);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'C');
    libertar_tabuleiro(&t);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Projeto_Test_Suite", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Adicionar testes atualizados
    CU_add_test(suite, "teste_lerJogo", teste_lerJogo);
    CU_add_test(suite, "teste_pintarDeBranco", teste_pintarDeBranco);
    CU_add_test(suite, "teste_riscar", teste_riscar);
    CU_add_test(suite, "teste_formatoParaCoordenadas", teste_formatoParaCoordenadas);
    CU_add_test(suite, "teste_formatoParaCoordenadas_extendido", teste_formatoParaCoordenadas_extendido);
    CU_add_test(suite, "teste_imprimirTabuleiro", teste_imprimirTabuleiro);
    CU_add_test(suite, "teste_imprimir_comandos", teste_imprimir_comandos);
    CU_add_test(suite, "teste_gerenciamento_estado", teste_gerenciamento_estado);
    CU_add_test(suite, "teste_verificacao_regras", teste_verificacao_regras);
    CU_add_test(suite, "teste_arquivos", teste_arquivos);
    CU_add_test(suite, "teste_stacks_cheia", teste_stacks_cheia);
    CU_add_test(suite, "teste_desfazer_vazio", teste_desfazer_vazio);
    CU_add_test(suite, "teste_gravarStack_erro_abertura", teste_gravarStack_erro_abertura);
    CU_add_test(suite, "teste_gravarJogo_erro_abertura", teste_gravarJogo_erro_abertura);
    CU_add_test(suite, "teste_lerStack_header_malformado", teste_lerStack_header_malformado);
    CU_add_test(suite, "teste_lerStack_linha_incompleta", teste_lerStack_linha_incompleta);
    CU_add_test(suite, "teste_verificar_riscadas_borda", teste_verificar_riscadas_borda);
    CU_add_test(suite, "teste_verificar_riscadas_interior", teste_verificar_riscadas_interior);
    CU_add_test(suite, "teste_verificar_brancas_coluna", teste_verificar_brancas_coluna);
    CU_add_test(suite, "teste_verificar_brancas_coluna_dupla", teste_verificar_brancas_coluna_dupla);
    CU_add_test(suite, "teste_lerJogo_header_malformado", teste_lerJogo_header_malformado);
    CU_add_test(suite, "teste_aplicar_correcoes", teste_aplicar_correcoes);
    CU_add_test(suite, "teste_resolve_jogo", teste_resolve_jogo);
    CU_add_test(suite, "teste_verificar_conectividade_isoladas", teste_verificar_conectividade_isoladas);
    CU_add_test(suite, "teste_formatoParaCoordenadas_NULL", teste_formatoParaCoordenadas_NULL);
    CU_add_test(suite, "teste_pintar_vizinhos_de_branco", teste_pintar_vizinhos_de_branco);
    CU_add_test(suite, "teste_risco_isolamento", teste_risco_isolamento);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
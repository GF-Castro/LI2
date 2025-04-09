#include "projeto.h" 
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>

// Teste para a função lerEstadoJogo
void test_lerEstadoJogo() {
    char tabuleiro[1000][1000];
    int linhas, colunas;

    // Criar um ficheiro temporário para simular a entrada
    FILE *f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "3 3\nA B C\nD E F\nG H I\n");
    fclose(f);

    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);

    // Verificar se as dimensões estão corretas
    CU_ASSERT_EQUAL(linhas, 3);
    CU_ASSERT_EQUAL(colunas, 3);

    // Verificar se o tabuleiro foi lido corretamente
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(tabuleiro[1][1], 'E');
    CU_ASSERT_EQUAL(tabuleiro[2][2], 'I');

    // Limpar o ficheiro temporário
    remove("test_tabuleiro.txt");
}

// Teste para a função pintarDeBranco
void test_pintarDeBranco() {
    char tabuleiro[1000][1000] = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    int linhas = 3, colunas = 3;

    pintarDeBranco(tabuleiro, linhas, colunas, 1, 1);

    CU_ASSERT_EQUAL(tabuleiro[1][1], 'E');
}

// Teste para a função riscar
void test_riscar() {
    char tabuleiro[1000][1000] = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    int linhas = 3, colunas = 3;

    riscar(tabuleiro, linhas, colunas, 2, 2);

    CU_ASSERT_EQUAL(tabuleiro[2][2], '#');
}

// Teste para a função formatoParaCoordenadas
void test_formatoParaCoordenadas() {
    int x, y;

    CU_ASSERT_TRUE(formatoParaCoordenadas("b2", &x, &y));
    CU_ASSERT_EQUAL(x, 1);
    CU_ASSERT_EQUAL(y, 1);

    CU_ASSERT_FALSE(formatoParaCoordenadas("invalido", &x, &y));
}

int main() {
    // Inicializar o registo de testes do CUnit
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Criar um conjunto de testes
    CU_pSuite suite = CU_add_suite("Projeto_Test_Suite", 0, 0);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Adicionar testes ao conjunto
    if ((NULL == CU_add_test(suite, "test_lerEstadoJogo", test_lerEstadoJogo)) ||
        (NULL == CU_add_test(suite, "test_pintarDeBranco", test_pintarDeBranco)) ||
        (NULL == CU_add_test(suite, "test_riscar", test_riscar)) ||
        (NULL == CU_add_test(suite, "test_formatoParaCoordenadas", test_formatoParaCoordenadas))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Executar os testes usando a interface básica
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Limpar o registo
    CU_cleanup_registry();
    return CU_get_error();
}
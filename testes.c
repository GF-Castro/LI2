#include "projeto.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>

// Teste para a função lerEstadoJogo
void teste_lerEstadoJogo() {
    char tabuleiro[1000][1000];
    int linhas, colunas;

    // Caso de teste: ficheiro com dimensões e tabuleiro válidos
    FILE *f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "3 3\nA B C\nD E F\nG H I\n");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    CU_ASSERT_EQUAL(linhas, 3);
    CU_ASSERT_EQUAL(colunas, 3);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'A');
    CU_ASSERT_EQUAL(tabuleiro[1][1], 'E');
    CU_ASSERT_EQUAL(tabuleiro[2][2], 'I');
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro vazio
    f = fopen("test_tabuleiro.txt", "w");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    remove("test_tabuleiro.txt");

    // Caso de teste: dimensões mal formatadas
    f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "X Y\nA B C\nD E F\nG H I\n");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    remove("test_tabuleiro.txt");

    // Caso de teste: dimensões muito grandes
    f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "1001 1001\n");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    remove("test_tabuleiro.txt");

    // Caso de teste: tabuleiro com caracteres não alfanuméricos
    f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "3 3\n@ # $\n%% ^ &\n* ( )\n");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    CU_ASSERT_EQUAL(tabuleiro[0][0], '@');
    CU_ASSERT_EQUAL(tabuleiro[1][1], '^');
    CU_ASSERT_EQUAL(tabuleiro[2][2], ')');
    remove("test_tabuleiro.txt");

    // Caso de teste: tabuleiro com linhas incompletas
    f = fopen("test_tabuleiro.txt", "w");
    fprintf(f, "3 3\nA B\nC D E\nF G H\n");
    fclose(f);
    lerEstadoJogo("test_tabuleiro.txt", tabuleiro, &linhas, &colunas);
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro inexistente
    lerEstadoJogo("nao_existe.txt", tabuleiro, &linhas, &colunas);
}

// Teste para a função pintarDeBranco
void teste_pintarDeBranco() {
    char tabuleiro[1000][1000] = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    int linhas = 3, colunas = 3;

    // Caso de teste: dentro dos limites
    pintarDeBranco(tabuleiro, linhas, colunas, 1, 1);
    CU_ASSERT_EQUAL(tabuleiro[1][1], 'E');

    // Caso de teste: fora dos limites (negativos)
    pintarDeBranco(tabuleiro, linhas, colunas, -1, -1);

    // Caso de teste: fora dos limites (maiores que o tamanho do tabuleiro)
    pintarDeBranco(tabuleiro, linhas, colunas, 100, 100);

    // Caso de teste: célula já em maiúscula
    pintarDeBranco(tabuleiro, linhas, colunas, 0, 0);
    CU_ASSERT_EQUAL(tabuleiro[0][0], 'A');

    // Caso de teste: tabuleiro vazio
    char tabuleiro_vazio[1000][1000] = {{0}};
    pintarDeBranco(tabuleiro_vazio, linhas, colunas, 1, 1);
    CU_ASSERT_EQUAL(tabuleiro_vazio[1][1], '\0');
}

// Teste para a função riscar
void teste_riscar() {
    char tabuleiro[1000][1000] = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    int linhas = 3, colunas = 3;

    // Caso de teste: dentro dos limites
    riscar(tabuleiro, linhas, colunas, 2, 2);
    CU_ASSERT_EQUAL(tabuleiro[2][2], '#');

    // Caso de teste: fora dos limites (negativos)
    riscar(tabuleiro, linhas, colunas, -1, -1);

    // Caso de teste: fora dos limites (maiores que o tamanho do tabuleiro)
    riscar(tabuleiro, linhas, colunas, 100, 100);

    // Caso de teste: célula já riscada
    riscar(tabuleiro, linhas, colunas, 2, 2);
    CU_ASSERT_EQUAL(tabuleiro[2][2], '#');

    // Caso de teste: tabuleiro vazio
    char tabuleiro_vazio[1000][1000] = {{0}};
    riscar(tabuleiro_vazio, linhas, colunas, 1, 1);
    CU_ASSERT_EQUAL(tabuleiro_vazio[1][1], '\0');
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

// Teste para a função imprimirTabuleiro
void teste_imprimirTabuleiro() {
    char tabuleiro[1000][1000] = {
        {'A', 'B', 'C'},
        {'D', 'E', 'F'},
        {'G', 'H', 'I'}
    };
    int linhas = 3, colunas = 3;

    // Caso de teste: tabuleiro preenchido
    imprimirTabuleiro(tabuleiro, linhas, colunas);

    // Caso de teste: tabuleiro vazio
    char tabuleiro_vazio[1000][1000] = {{0}};
    imprimirTabuleiro(tabuleiro_vazio, linhas, colunas);
}

// Teste para a função imprimir_comandos
void teste_imprimir_comandos() {
    // Apenas chamada para verificar ausência de erros
    imprimir_comandos();
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
    if ((NULL == CU_add_test(suite, "teste_lerEstadoJogo", teste_lerEstadoJogo)) ||
        (NULL == CU_add_test(suite, "teste_pintarDeBranco", teste_pintarDeBranco)) ||
        (NULL == CU_add_test(suite, "teste_riscar", teste_riscar)) ||
        (NULL == CU_add_test(suite, "teste_formatoParaCoordenadas", teste_formatoParaCoordenadas)) ||
        (NULL == CU_add_test(suite, "teste_imprimirTabuleiro", teste_imprimirTabuleiro)) ||
        (NULL == CU_add_test(suite, "teste_imprimir_comandos", teste_imprimir_comandos))) {
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

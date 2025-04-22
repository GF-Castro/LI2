#include "projeto.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>

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
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro vazio
    f = fopen("test_tabuleiro.txt", "w");
    fclose(f);
    lerJogo("test_tabuleiro.txt", &t);
    remove("test_tabuleiro.txt");

    // Caso de teste: ficheiro inexistente
    lerJogo("nao_existe.txt", &t);
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

// Teste para a função formatoParaCoordenadas com casos adicionais
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

// Teste para as funções de gerenciamento de estado (stack)
void teste_gerenciamento_estado() {
    Tabuleiro t1 = {{{'a','b'}, {'c','d'}}, 2, 2};
    Tabuleiro t2 = {{{'e','f'}, {'g','h'}}, 2, 2};
    
    // Teste empilhar e desempilhar
    stacks(t1);
    CU_ASSERT_EQUAL(topoStack, 0);
    stacks(t2);
    CU_ASSERT_EQUAL(topoStack, 1);
    
    Tabuleiro t_recuperado = desempilhar();
    CU_ASSERT_EQUAL(t_recuperado.linhas, 2);
    CU_ASSERT_EQUAL(t_recuperado.tabuleiro[0][0], 'e');
    CU_ASSERT_EQUAL(topoStack, 0);
    
    t_recuperado = desempilhar();
    CU_ASSERT_EQUAL(t_recuperado.linhas, 2);
    CU_ASSERT_EQUAL(t_recuperado.tabuleiro[0][0], 'a');
    CU_ASSERT_EQUAL(topoStack, -1);
    
    // Teste desempilhar com stack vazia
    t_recuperado = desempilhar();
    CU_ASSERT_EQUAL(t_recuperado.linhas, 0);
    
    // Teste guardar_estado e desfazer
    Tabuleiro t_atual = {{{'x','y'}, {'z','w'}}, 2, 2};
    guardar_estado(&t_atual);
    CU_ASSERT_EQUAL(topoStack, 0);
    
    t_atual.tabuleiro[0][0] = 'm';
    desfazer(&t_atual);
    CU_ASSERT_EQUAL(t_atual.tabuleiro[0][0], 'x');
    CU_ASSERT_EQUAL(topoStack, -1);
}

// Teste para as funções de verificação de regras
void teste_verificacao_regras() {
    char tabuleiro_valido[1000][1000] = {{0}};
    char tabuleiro_invalido[1000][1000] = {{0}};
    
    // Preencher com dados de teste
    tabuleiro_valido[0][0] = 'A'; tabuleiro_valido[0][1] = '#'; tabuleiro_valido[0][2] = 'B';
    tabuleiro_valido[1][0] = '#'; tabuleiro_valido[1][1] = 'C'; tabuleiro_valido[1][2] = '#';
    tabuleiro_valido[2][0] = 'D'; tabuleiro_valido[2][1] = '#'; tabuleiro_valido[2][2] = 'E';
    
    tabuleiro_invalido[0][0] = 'A'; tabuleiro_invalido[0][1] = '#'; tabuleiro_invalido[0][2] = 'A';
    tabuleiro_invalido[1][0] = '#'; tabuleiro_invalido[1][1] = 'B'; tabuleiro_invalido[1][2] = '#';
    tabuleiro_invalido[2][0] = 'C'; tabuleiro_invalido[2][1] = '#'; tabuleiro_invalido[2][2] = 'B';
    
    // Testes
    verificar_riscadas(tabuleiro_valido, 3, 3);
    verificar_brancas(tabuleiro_valido, 3, 3);
    verificar_riscadas(tabuleiro_invalido, 3, 3);
    verificar_brancas(tabuleiro_invalido, 3, 3);
    verificar_estado(tabuleiro_valido, 3, 3);
    verificar_estado(tabuleiro_invalido, 3, 3);
}

// Teste para as funções de gravação e leitura de arquivo
void teste_arquivos() {
    Tabuleiro t_original = {{{'a','b'}, {'c','d'}}, 2, 2};
    Tabuleiro t_lido = {0};
    
    // Teste gravar e ler jogo
    gravarJogo("teste_jogo.txt", &t_original);
    lerJogo("teste_jogo.txt", &t_lido);
    CU_ASSERT_EQUAL(t_lido.linhas, 2);
    CU_ASSERT_EQUAL(t_lido.colunas, 2);
    CU_ASSERT_EQUAL(t_lido.tabuleiro[0][0], 'a');
    remove("teste_jogo.txt");
    remove("stack.txt");
    
    // Teste gravar e ler stack
    stacks(t_original);
    gravarStack("teste_stack.txt");
    topoStack = -1; // Reset para simular novo programa
    lerStack("teste_stack.txt");
    CU_ASSERT_EQUAL(topoStack, 0);
    CU_ASSERT_EQUAL(stack[0].linhas, 2);
    remove("teste_stack.txt");
    
    // Teste com arquivos inexistentes
    lerJogo("nao_existe.txt", &t_lido);
    lerStack("nao_existe.txt");
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
    if ((NULL == CU_add_test(suite, "teste_lerJogo", teste_lerJogo)) ||
        (NULL == CU_add_test(suite, "teste_pintarDeBranco", teste_pintarDeBranco)) ||
        (NULL == CU_add_test(suite, "teste_riscar", teste_riscar)) ||
        (NULL == CU_add_test(suite, "teste_formatoParaCoordenadas", teste_formatoParaCoordenadas)) ||
        (NULL == CU_add_test(suite, "teste_formatoParaCoordenadas_extendido", teste_formatoParaCoordenadas_extendido)) ||
        (NULL == CU_add_test(suite, "teste_imprimirTabuleiro", teste_imprimirTabuleiro)) ||
        (NULL == CU_add_test(suite, "teste_imprimir_comandos", teste_imprimir_comandos)) ||
        (NULL == CU_add_test(suite, "teste_gerenciamento_estado", teste_gerenciamento_estado)) ||
        (NULL == CU_add_test(suite, "teste_verificacao_regras", teste_verificacao_regras)) ||
        (NULL == CU_add_test(suite, "teste_arquivos", teste_arquivos))) {
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
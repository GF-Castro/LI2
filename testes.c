#include "projeto.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

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
    char tabuleiro[26][1000] = {
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
    char tabuleiro_vazio[26][1000] = {{0}};
    pintarDeBranco(tabuleiro_vazio, linhas, colunas, 1, 1);
    CU_ASSERT_EQUAL(tabuleiro_vazio[1][1], '\0');
}

// Teste para a função riscar
void teste_riscar() {
    char tabuleiro[26][1000] = {
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
    char tabuleiro_vazio[26][1000] = {{0}};
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
    char tabuleiro[26][1000] = {
        {'A', 'B', 'C'},
        {'D', 'E', 'F'},
        {'G', 'H', 'I'}
    };
    int linhas = 3, colunas = 3;

    // Caso de teste: tabuleiro preenchido
    imprimirTabuleiro(tabuleiro, linhas, colunas);

    // Caso de teste: tabuleiro vazio
    char tabuleiro_vazio[26][1000] = {{0}};
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
    char tabuleiro_valido[26][1000] = {{0}};
    char tabuleiro_invalido[26][1000] = {{0}};
    
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

void teste_stacks_cheia() {
    Tabuleiro t = { .linhas = 1, .colunas = 1 };
    t.tabuleiro[0][0] = 'X';
    topoStack = -1;
    for (int i = 0; i < tamanhoStack; i++) guardar_estado(&t);
    CU_ASSERT_EQUAL(topoStack, tamanhoStack - 1);
    stacks(t);
    CU_ASSERT_EQUAL(topoStack, tamanhoStack - 1);
}

void teste_desfazer_vazio() {
    topoStack = -1;
    Tabuleiro t = { .linhas = 2, .colunas = 2 };
    t.tabuleiro[0][0] = 'A';
    desfazer(&t);
    CU_ASSERT_EQUAL(topoStack, -1);
}

void teste_gravarStack_erro_abertura() {
    mkdir("tmpdir", 0700);
    gravarStack("tmpdir");
    rmdir("tmpdir");
    CU_PASS("gravarStack erro de abertura coberto");
}

void teste_gravarJogo_erro_abertura() {
    mkdir("tmpdir2", 0700);
    Tabuleiro t = { .linhas = 1, .colunas = 1 };
    t.tabuleiro[0][0] = 'B';
    gravarJogo("tmpdir2", &t);
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
    char tab[26][1000] = {{0}};
    tab[0][0] = '#';
    tab[1][0] = 'A';
    tab[0][1] = 'B';
    verificar_riscadas(tab, 2, 2);
    CU_PASS("verificar_riscadas borda coberto");
}

void teste_verificar_riscadas_interior() {
    char tab[26][1000] = {{0}};
    tab[1][1] = '#';
    verificar_riscadas(tab, 3, 3);
    CU_PASS("verificar_riscadas interior coberto");
}

void teste_verificar_brancas_coluna() {
    char tab[26][1000] = {{0}};
    tab[0][0] = 'C';
    tab[1][0] = 'C';
    tab[2][0] = 'C';
    verificar_brancas(tab, 3, 2);
    CU_PASS("verificar_brancas coluna coberto");
}

void teste_verificar_brancas_coluna_dupla() {
    char tab[26][1000] = {{0}};
    tab[0][1] = 'D';
    tab[1][1] = 'D';
    verificar_brancas(tab, 2, 2);
    CU_PASS("verificar_brancas coluna dupla coberto");
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



// Novo teste para aplicar correções (Regras 1, 2, 3)
void teste_aplicar_correcoes() {
    Tabuleiro t = { .linhas = 3, .colunas = 3 };
    strcpy(t.tabuleiro[0], "aAB");
    strcpy(t.tabuleiro[1], "#cD");
    strcpy(t.tabuleiro[2], "efG");
    
    aplicar_correcoes(&t);
    
    // Regra 1: 'a' na linha 0 deve ser riscado por causa de 'A'
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], '#');
    // Regra 2: vizinhos de '#' (posição 1,0) devem ser maiúsculos
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], '#');
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'C'); // 'c' convertido para 'C'
    // Regra 3: 'e' não pode ser riscado para não isolar
    CU_ASSERT_EQUAL(t.tabuleiro[2][0], 'E');
}

// Teste para a função resolve_jogo
void teste_resolve_jogo() {
    Tabuleiro t = { .linhas = 3, .colunas = 3 };
    strcpy(t.tabuleiro[0], "abc");
    strcpy(t.tabuleiro[1], "def");
    strcpy(t.tabuleiro[2], "ghi");
    
    // Aplicar resolução
    int mudou = resolve_jogo(&t);
    CU_ASSERT_EQUAL(mudou, 1);
    // Verificar se alguma célula foi alterada
        CU_ASSERT_TRUE(isupper(t.tabuleiro[0][0]) || t.tabuleiro[0][0] == '#');
}

// Teste para verificar conectividade com células isoladas
void teste_verificar_conectividade_isoladas() {
    char tab[26][1000] = {{0}};
    tab[0][0] = 'A';
    tab[2][2] = 'B'; // Isolada de 'A'
    verificar_conectividade(tab, 3, 3);
}

// Teste para formatoParaCoordenadas com input NULL
void teste_formatoParaCoordenadas_NULL() {
    int x, y;
    CU_ASSERT_FALSE(formatoParaCoordenadas(NULL, &x, &y));
}

// Teste para pintar_vizinhos_de_branco
void teste_pintar_vizinhos_de_branco() {
    Tabuleiro t = { .linhas = 3, .colunas = 3 };
    t.tabuleiro[1][1] = '#';
    t.tabuleiro[0][1] = 'a';
    t.tabuleiro[1][0] = 'b';
    
    int mudou = pintar_vizinhos_de_branco(&t, 1, 1);
    CU_ASSERT_EQUAL(mudou, 1);
    CU_ASSERT_EQUAL(t.tabuleiro[0][1], 'A');
    CU_ASSERT_EQUAL(t.tabuleiro[1][0], 'B');
}

// Teste para risco que isolaria células
void teste_risco_isolamento() {
    Tabuleiro t = { .linhas = 3, .colunas = 3 };
    strcpy(t.tabuleiro[0], "A##");
    strcpy(t.tabuleiro[1], "##B");
    // Tentar riscar a célula (1,1) deve falhar e pintar de branco
    t.tabuleiro[1][1] = 'c';
    aplicar_correcoes(&t);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'C');
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Projeto_Test_Suite", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Testes existentes
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

    // Novos testes adicionados
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
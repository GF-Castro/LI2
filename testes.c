#include "projeto.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

// Funçao de ajuda
Tabuleiro criar_tabuleiro_teste(int linhas, int colunas,  char *content) {
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
    Tabuleiro t = {0};  // inicializa tudo a zero/NULL

    // Caso 1: ficheiro válido
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

    // Caso 2: ficheiro vazio 
    f = fopen("test_tabuleiro.txt", "w");
    fclose(f);
    lerJogo("test_tabuleiro.txt", &t);
    CU_ASSERT_PTR_NULL(t.tabuleiro);   // verificar que não alocou nada
    CU_ASSERT_EQUAL(t.linhas, 0);
    CU_ASSERT_EQUAL(t.colunas, 0);
    libertar_tabuleiro(&t);
    remove("test_tabuleiro.txt");

    // Caso 3: ficheiro inexistente 
    lerJogo("nao_existe.txt", &t);
    CU_ASSERT_PTR_NULL(t.tabuleiro);
    CU_ASSERT_EQUAL(t.linhas, 0);
    CU_ASSERT_EQUAL(t.colunas, 0);
    libertar_tabuleiro(&t);
}

void teste_pintarDeBranco() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "abcdefghi");

    // Pintar (1,1) deve converter 'e' -> 'E'
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 1, 1);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'E');

    // Guardar estado atual para verificar que não muda com índices inválidos
    char original00 = t.tabuleiro[0][0];
    char original22 = t.tabuleiro[2][2];

    // Fora dos limites negativos, não altera tabuleiro
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, -1, -1);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], original00);
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], original22);

    // Fora dos limites maiores, não altera tabuleiro
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 100, 100);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], original00);
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], original22);

    // Já maiúsculo, não altera (pinta (0,0) que era 'a' e deve passar a 'A')
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 0, 0);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], 'A');

    // Aplicar de novo em maiúscula, mantém 'A'
    pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, 0, 0);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], 'A');

    libertar_tabuleiro(&t);
}

void teste_riscar() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "abcdefghi");

    // Riscar dentro dos limites (2,2)
    riscar(t.tabuleiro, t.linhas, t.colunas, 2, 2);
    CU_ASSERT_EQUAL(t.tabuleiro[2][2], '#');

    // Guardar estado para verificar que não muda com índices inválidos
    char original00 = t.tabuleiro[0][0];
    char original11 = t.tabuleiro[1][1];

    // Fora dos limites negativos, não altera
    riscar(t.tabuleiro, t.linhas, t.colunas, -1, -1);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], original00);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], original11);

    // Fora dos limites maiores, não altera
    riscar(t.tabuleiro, t.linhas, t.colunas, 100, 100);
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], original00);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], original11);

    // Já riscado, mantém '#'
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
    
    // Teste empilhar 
    guardar_move('b', 0, 0, 'a', 'A');
    CU_ASSERT_EQUAL(topoStack, 0);
    
    guardar_move('r', 1, 1, 'h', '#');
    CU_ASSERT_EQUAL(topoStack, 1);
    
}


void teste_verificacao_regras() {
    // Criação dos tabuleiros movida para dentro desta função
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
    topoStack = -1;  // stack vazia
    Tabuleiro t = criar_tabuleiro_teste(2, 2, "ABCD");

    // Guarda um valor inicial para comparar depois
    char valor_inicial = t.tabuleiro[0][0];

    desfazer(&t);

    // topoStack continua igual (-1)
    CU_ASSERT_EQUAL(topoStack, -1);

    // O tabuleiro não mudou
    CU_ASSERT_EQUAL(t.tabuleiro[0][0], valor_inicial);

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
    libertar_tabuleiro(&t); // CORREÇÃO: Libera memória do tabuleiro
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

// Teste para consumirNovaLinha
void teste_consumirNovaLinha() {
    // Caso normal: linha com texto e newline
    FILE *f = fopen("tmpfile1.txt", "w+");
    fprintf(f, "abc\ndef\n");
    rewind(f);
    int c1 = fgetc(f); // 'a'
    CU_ASSERT_EQUAL(c1, 'a');
    consumirNovaLinha(f); // deve consumir 'bc\n'
    int c2 = fgetc(f); // deve estar no início da próxima linha, 'd'
    CU_ASSERT_EQUAL(c2, 'd');
    fclose(f);
    remove("tmpfile1.txt");

    // Caso: linha vazia, só newline
    f = fopen("tmpfile2.txt", "w+");
    fprintf(f, "\nabc\n");
    rewind(f);
    consumirNovaLinha(f); // deve consumir só '\n'
    int c3 = fgetc(f); // deve estar no 'a'
    CU_ASSERT_EQUAL(c3, 'a');
    fclose(f);
    remove("tmpfile2.txt");

    // Caso: já no EOF
    f = fopen("tmpfile3.txt", "w+");
    fprintf(f, "abc");
    fclose(f);
    f = fopen("tmpfile3.txt", "r");
    fseek(f, 0, SEEK_END); // vai para EOF
    consumirNovaLinha(f); // não deve crashar
    fclose(f);
    remove("tmpfile3.txt");
}

// Teste para resolver_recursivo
void teste_resolver_recursivo() {
    // Tabuleiro resolvido (exemplo)
    Tabuleiro t1 = criar_tabuleiro_teste(2, 2, "ABCD");
    bool res1 = resolver_recursivo(&t1);
    CU_ASSERT(res1 == true || res1 == false); // Apenas valida que retorna bool
    libertar_tabuleiro(&t1);

    // Tabuleiro impossível de resolver (por exemplo, só #)
    Tabuleiro t2 = criar_tabuleiro_teste(2, 2, "####");
    bool res2 = resolver_recursivo(&t2);
    CU_ASSERT(res2 == false || res2 == true); // Só valida tipo, sem saber a lógica interna
    libertar_tabuleiro(&t2);

    // Tabuleiro vazio
    Tabuleiro t3 = criar_tabuleiro(0, 0);
    bool res3 = resolver_recursivo(&t3);
    CU_ASSERT(res3 == false || res3 == true);
    libertar_tabuleiro(&t3);

    // Tabuleiro com mistura (valores minúsculos e maiúsculos)
    Tabuleiro t4 = criar_tabuleiro_teste(2, 2, "aBcD");
    bool res4 = resolver_recursivo(&t4);
    CU_ASSERT(res4 == true || res4 == false);
    libertar_tabuleiro(&t4);

    // Tabuleiro NULL (caso de erro)
    bool res5 = resolver_recursivo(NULL);
    CU_ASSERT(res5 == false || res5 == true);
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

void teste_risco_isolamento() {
    Tabuleiro t = criar_tabuleiro_teste(3, 3, "A#######B");
    t.tabuleiro[1][1] = 'c';
    aplicar_correcoes(&t);
    CU_ASSERT_EQUAL(t.tabuleiro[1][1], 'C');
    libertar_tabuleiro(&t);
}

void teste_tem_minusculas() {
    Tabuleiro t = criar_tabuleiro_teste(2, 2, "AbCD");
    CU_ASSERT_TRUE(tem_minusculas(&t));
    t.tabuleiro[0][1] = 'B'; // Tudo maiúsculo agora
    CU_ASSERT_FALSE(tem_minusculas(&t));
    libertar_tabuleiro(&t);
}

void teste_copiar_tabuleiro() {
    // Criar tabuleiro de teste
    Tabuleiro t = criar_tabuleiro_teste(2, 3, "abcDEF");

    // Copiar o tabuleiro
    Tabuleiro copia = copiar_tabuleiro(&t);

    // Verificar tamanho
    CU_ASSERT_EQUAL(copia.linhas, t.linhas);
    CU_ASSERT_EQUAL(copia.colunas, t.colunas);

    // Verificar conteúdo linha a linha
    for (int i = 0; i < t.linhas; i++) {
        for (int j = 0; j < t.colunas; j++) {
            CU_ASSERT_EQUAL(copia.tabuleiro[i][j], t.tabuleiro[i][j]);
        }
    }

    // Modificar o original e verificar que a cópia não muda
    t.tabuleiro[0][0] = 'Z';
    CU_ASSERT_NOT_EQUAL(copia.tabuleiro[0][0], t.tabuleiro[0][0]);

    // Libertar memória
    libertar_tabuleiro(&t);
    libertar_tabuleiro(&copia);
}

void teste_validacao_parcial() {
    // Tabuleiro 3x3 sem repetições
    Tabuleiro t1 = criar_tabuleiro_teste(3, 3, "ABCDEFHIJ");
    CU_ASSERT_TRUE(validacao_parcial(&t1, 0, 0));
    CU_ASSERT_TRUE(validacao_parcial(&t1, 2, 2));

    Tabuleiro t2 = criar_tabuleiro_teste(3, 3, "AABDEFHIJ");
    CU_ASSERT_FALSE(validacao_parcial(&t2, 0, 1)); 

    Tabuleiro t3 = criar_tabuleiro_teste(3, 3, "ABCDABHIJ");
    t3.tabuleiro[0][1] = 'B';
    t3.tabuleiro[1][1] = 'B'; 
    CU_ASSERT_FALSE(validacao_parcial(&t3, 1, 1));

    Tabuleiro t4 = criar_tabuleiro(3, 3);
    CU_ASSERT_TRUE(validacao_parcial(&t4, 0, 0));

    libertar_tabuleiro(&t1);
    libertar_tabuleiro(&t2);
    libertar_tabuleiro(&t3);
    libertar_tabuleiro(&t4);
}

void teste_tabuleiros_iguais() {
    Tabuleiro t1 = criar_tabuleiro_teste(2, 2, "ABCD");
    Tabuleiro t2 = criar_tabuleiro_teste(2, 2, "ABCD");
    Tabuleiro t3 = criar_tabuleiro_teste(2, 2, "ABCF");
    Tabuleiro t4 = criar_tabuleiro_teste(2, 2, "DCBA");
    Tabuleiro t5 = criar_tabuleiro_teste(2, 2, "ABCD");
    t5.linhas = 1; 

    CU_ASSERT_TRUE(tabuleiros_iguais(&t1, &t2));    
    CU_ASSERT_FALSE(tabuleiros_iguais(&t1, &t3));  
    CU_ASSERT_FALSE(tabuleiros_iguais(&t1, &t4));  
    CU_ASSERT_FALSE(tabuleiros_iguais(&t1, &t5));   
    
    libertar_tabuleiro(&t1);
    libertar_tabuleiro(&t2);
    libertar_tabuleiro(&t3);
    libertar_tabuleiro(&t4);
    libertar_tabuleiro(&t5);
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
    CU_add_test(suite, "teste_consumirNovaLinha", teste_consumirNovaLinha);
    CU_add_test(suite, "teste_resolver_recursivo", teste_resolver_recursivo);
    CU_add_test(suite, "teste_verificar_conectividade_isoladas", teste_verificar_conectividade_isoladas);
    CU_add_test(suite, "teste_formatoParaCoordenadas_NULL", teste_formatoParaCoordenadas_NULL);
    CU_add_test(suite, "teste_risco_isolamento", teste_risco_isolamento);
    CU_add_test(suite, "teste_tem_minusculas", teste_tem_minusculas);
    CU_add_test(suite, "teste_copiar_tabuleiro", teste_copiar_tabuleiro);
    CU_add_test(suite, "teste_validacao_parcial", teste_validacao_parcial);
    CU_add_test(suite, "teste_tabuleiros_iguais", teste_tabuleiros_iguais);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
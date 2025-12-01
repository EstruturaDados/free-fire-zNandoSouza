#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// --- CONFIGURAÇÕES GLOBAIS ---
#define MAX_ITENS 10    // Vetor estático com capacidade para até 10 itens
#define MAX_NOME 50
#define MAX_TIPO 30

// --- ENUMERAÇÃO (Nível Mestre) ---
// Define os possíveis critérios de ordenação
typedef enum {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

// --- ESTRUTURA (Nível Novato) ---
// Struct para representar um item de sobrevivência
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // 1 (Mais Importante) a 5 (Menos Importante) - Nível Mestre
} Item;

// --- VARIÁVEIS DE ESTADO ---
Item mochila[MAX_ITENS];
int contador_itens = 0;
// Flag de controle para a Busca Binária (Nível Mestre)
bool ordenado_por_nome = false; 

// --- PROTÓTIPOS DAS FUNÇÕES ---
void exibir_menu();
void adicionar_item();
void remover_item();
void listar_itens();
void buscar_item_sequencial();
void buscar_item_binaria();
void ordenar_mochila();
void insertion_sort(CriterioOrdenacao criterio);


// =================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// =================================================================
int main() {
    int opcao;
    
    // Boa prática para garantir que o printf exiba o texto imediatamente
    setbuf(stdout, NULL); 

    do {
        exibir_menu();
        
        // Validação básica de entrada para evitar crash
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa o buffer de entrada
            printf("⚠️ Entrada inválida. Por favor, digite um número da opção.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                adicionar_item();
                break;
            case 2:
                remover_item();
                break;
            case 3:
                listar_itens();
                break;
            case 4:
                buscar_item_sequencial(); // Nível Aventureiro
                break;
            case 5:
                ordenar_mochila();        // Nível Mestre
                break;
            case 6:
                buscar_item_binaria();    // Nível Mestre
                break;
            case 0:
                printf("\n👋 Sobrevivência Garantida! Desligando sistema da MateCheck.\n");
                break;
            default:
                printf("\n❌ Opção fora do escopo. Tente um número de 0 a 6.\n");
        }
    } while (opcao != 0);

    return 0;
}


// =================================================================
// FUNÇÕES DE INTERFACE E UTILIDADE
// =================================================================

/**
 * Exibe o menu principal de ações.
 */
void exibir_menu() {
    printf("\n--- 🔫 Mochila Virtual MateCheck (Nível Mestre) ---\n");
    printf("1. Adicionar Novo Item\n");
    printf("2. Remover Item por Nome\n");
    printf("3. Listar Itens Cadastrados\n");
    printf("-------------------------------------------------\n");
    printf("4. [Aventureiro] Buscar Item por Nome (Sequencial)\n");
    printf("5. [Mestre] Ordenar Mochila (Nome, Tipo ou Prioridade)\n");
    printf("6. [Mestre] Buscar Item por Nome (Binária)\n");
    printf("-------------------------------------------------\n");
    printf("0. Sair do Sistema\n");
    printf("Escolha sua ação: ");
}


// =================================================================
// NÍVEL NOVATO: CRUD BÁSICO
// =================================================================

/**
 * Adiciona um novo item à mochila, lendo todos os campos, incluindo Prioridade.
 */
void adicionar_item() {
    if (contador_itens >= MAX_ITENS) {
        printf("\n🚫 Mochila Cheia! (Capacidade: %d)\n", MAX_ITENS);
        return;
    }

    Item novo_item;
    printf("\n--- [1] Adicionar Novo Item ---\n");
    
    // Leitura do Nome e Tipo (usando %49[^\n] para ler strings com espaços)
    printf("Nome do item: ");
    scanf(" %49[^\n]", novo_item.nome);
    printf("Tipo do item (Arma, Curativo, Munição, etc.): ");
    scanf(" %29[^\n]", novo_item.tipo);

    // Leitura da Quantidade (com validação de entrada)
    printf("Quantidade: ");
    while (scanf("%d", &novo_item.quantidade) != 1 || novo_item.quantidade <= 0) {
        while (getchar() != '\n');
        printf("⚠️ Quantidade inválida. Digite um número positivo: ");
    }
    
    // Leitura da Prioridade (Nível Mestre - validação entre 1 e 5)
    printf("Prioridade (1=Mais, 5=Menos): ");
    while (scanf("%d", &novo_item.prioridade) != 1 || novo_item.prioridade < 1 || novo_item.prioridade > 5) {
        while (getchar() != '\n');
        printf("⚠️ Prioridade inválida. Digite um número inteiro entre 1 e 5: ");
    }

    // Adiciona o item e atualiza o contador
    mochila[contador_itens] = novo_item;
    contador_itens++;
    ordenado_por_nome = false; // Invalida o estado de ordenação por nome
    printf("\n✅ Item '%s' adicionado com sucesso na posição %d!\n", novo_item.nome, contador_itens);
}

/**
 * Remove um item da mochila pelo nome, deslocando os elementos.
 */
void remover_item() {
    if (contador_itens == 0) {
        printf("\n⚠️ A mochila está vazia. Nada para remover.\n");
        return;
    }

    char nome_busca[MAX_NOME];
    printf("\n--- [2] Remover Item ---\n");
    printf("Digite o nome exato do item a ser removido: ");
    scanf(" %49[^\n]", nome_busca);

    int i, encontrado_idx = -1;

    // Busca o item
    for (i = 0; i < contador_itens; i++) {
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            encontrado_idx = i;
            break;
        }
    }

    if (encontrado_idx != -1) {
        // Desloca os itens (cria o "buraco" no vetor)
        for (i = encontrado_idx; i < contador_itens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        contador_itens--;
        ordenado_por_nome = false; // Invalida o estado de ordenação por nome
        printf("\n🗑️ Item '%s' removido com sucesso!\n", nome_busca);
    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nome_busca);
    }
}

/**
 * Lista todos os itens cadastrados em formato de tabela.
 */
void listar_itens() {
    if (contador_itens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }

    printf("\n--- [3] 📋 Inventário de Sobrevivência (%d/%d) ---\n", contador_itens, MAX_ITENS);
    printf("+----------------------+------------------+------------+------------+\n");
    printf("| %-20s | %-16s | %-10s | %-10s |\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("+----------------------+------------------+------------+------------+\n");

    for (int i = 0; i < contador_itens; i++) {
        printf("| %-20s | %-16s | %-10d | %-10d |\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("+----------------------+------------------+------------+------------+\n");
    printf("Estado da Ordenação por Nome: %s\n", ordenado_por_nome ? "✅ Ordenado" : "⚠️ Desordenado");
}


// =================================================================
// NÍVEL AVENTUREIRO: BUSCA SEQUENCIAL
// =================================================================

/**
 * Realiza a busca sequencial por nome (Itera sobre todos os elementos).
 */
void buscar_item_sequencial() {
    if (contador_itens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }
    
    char nome_busca[MAX_NOME];
    printf("\n--- [4] Buscar Item (Sequencial) ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    scanf(" %49[^\n]", nome_busca);

    int i;
    bool encontrado = false; // Flag de controle

    for (i = 0; i < contador_itens; i++) {
        // Comparação de strings (strcmp)
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            printf("\n🔍 Item **ENCONTRADO** (Busca Sequencial):\n");
            printf("  - Nome: **%s**\n", mochila[i].nome);
            printf("  - Tipo: %s\n", mochila[i].tipo);
            printf("  - Quantidade: %d\n", mochila[i].quantidade);
            printf("  - Prioridade: %d\n", mochila[i].prioridade);
            encontrado = true;
            break; 
        }
    }

    if (!encontrado) {
        printf("\n❌ Item '%s' não encontrado. Tente novamente ou verifique a digitação.\n", nome_busca);
    }
}


// =================================================================
// NÍVEL MESTRE: ORDENAÇÃO E BUSCA BINÁRIA
// =================================================================

/**
 * Menu para escolher o critério de ordenação e chama a função Insertion Sort.
 */
void ordenar_mochila() {
    if (contador_itens <= 1) {
        printf("\n⚠️ É necessário ter pelo menos dois itens para ordenar.\n");
        return;
    }

    int escolha;
    printf("\n--- [5] Menu de Ordenação ---\n");
    printf("Escolha o critério para organização:\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo\n");
    printf("3. Por Prioridade (1=Mais, 5=Menos)\n");
    printf("Opção: ");

    if (scanf("%d", &escolha) != 1) {
        while (getchar() != '\n');
        printf("⚠️ Entrada inválida.\n");
        return;
    }

    switch (escolha) {
        case 1:
            insertion_sort(POR_NOME);
            printf("\n✨ Mochila ordenada por NOME com sucesso!\n");
            ordenado_por_nome = true; // Define o flag para a Busca Binária
            break;
        case 2:
            insertion_sort(POR_TIPO);
            printf("\n✨ Mochila ordenada por TIPO com sucesso!\n");
            ordenado_por_nome = false; 
            break;
        case 3:
            insertion_sort(POR_PRIORIDADE);
            printf("\n✨ Mochila ordenada por PRIORIDADE com sucesso!\n");
            ordenado_por_nome = false; 
            break;
        default:
            printf("\n❌ Opção de critério inválida.\n");
    }
}

/**
 * Implementação do Insertion Sort para ordenar a mochila.
 * Inclui o contador de comparações para análise de desempenho.
 */
void insertion_sort(CriterioOrdenacao criterio) {
    int i, j;
    Item chave;
    // Contador de comparações (requisito Nível Mestre)
    long long contador_comunicacoes = 0; 

    for (i = 1; i < contador_itens; i++) {
        chave = mochila[i];
        j = i - 1;

        // Loop de comparação e deslocamento
        while (j >= 0) {
            int resultado_comparacao = 0;
            contador_comunicacoes++; // Conta cada comparação dentro do loop

            // Lógica de comparação baseada no ENUM
            if (criterio == POR_NOME) {
                // strcmp retorna > 0 se o primeiro for 'maior' (alfabeticamente depois)
                resultado_comparacao = strcmp(mochila[j].nome, chave.nome);
            } else if (criterio == POR_TIPO) {
                resultado_comparacao = strcmp(mochila[j].tipo, chave.tipo);
            } else if (criterio == POR_PRIORIDADE) {
                // Ordena por prioridade crescente (1 deve vir antes de 5)
                resultado_comparacao = mochila[j].prioridade - chave.prioridade; 
            }

            if (resultado_comparacao > 0) { 
                // Move o elemento para a direita
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break; // A posição correta foi encontrada
            }
        }
        mochila[j + 1] = chave; // Insere o item na posição correta
    }
    
    printf("\n📈 Análise de Desempenho da Ordenação:\n");
    printf("   - Algoritmo: Insertion Sort\n");
    printf("   - Total de Itens: %d\n", contador_itens);
    printf("   - **Comparações Realizadas**: %lld\n", contador_comunicacoes);
}

/**
 * Implementação da Busca Binária, que exige que a lista esteja ordenada.
 */
void buscar_item_binaria() {
    if (contador_itens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }
    
    // Validação de pré-requisito (requisito Nível Mestre)
    if (!ordenado_por_nome) {
        printf("\n❌ A busca binária falhou!\n");
        printf("   Pré-requisito: A mochila DEVE estar **ordenada por nome**.\n");
        printf("   Use a Opção **5** e escolha o critério **1 (Por Nome)** antes de buscar.\n");
        return;
    }
    
    char nome_busca[MAX_NOME];
    printf("\n--- [6] Buscar Item (Binária) ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    scanf(" %49[^\n]", nome_busca);

    int esq = 0;
    int dir = contador_itens - 1;
    bool encontrado = false;
    int pos_encontrada = -1;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        int comparacao = strcmp(mochila[meio].nome, nome_busca);

        if (comparacao == 0) {
            // Encontrado!
            encontrado = true;
            pos_encontrada = meio;
            break;
        } else if (comparacao < 0) {
            // Item buscado é maior que o do meio (vai para a direita)
            esq = meio + 1;
        } else {
            // Item buscado é menor que o do meio (vai para a esquerda)
            dir = meio - 1;
        }
    }

    if (encontrado) {
        printf("\n🎯 Item **ENCONTRADO** (Busca Binária - Posição: %d):\n", pos_encontrada + 1);
        printf("  - Nome: **%s**\n", mochila[pos_encontrada].nome);
        printf("  - Tipo: %s\n", mochila[pos_encontrada].tipo);
        printf("  - Quantidade: %d\n", mochila[pos_encontrada].quantidade);
        printf("  - Prioridade: %d\n", mochila[pos_encontrada].prioridade);
    } else {
        printf("\n❌ Item '%s' não encontrado (Busca Binária).\n", nome_busca);
    }
}
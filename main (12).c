#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 100

typedef struct Contact {
    char name[100];
    char phone[20];
    struct Contact *next;
} Contact;

Contact *hash_table[TABLE_SIZE];

// Variáveis para medir desempenho
double total_insert_time = 0;
int insert_count = 0;
double total_search_time = 0;
int search_count = 0;

// soma dos valores ASCII dos caracteres do nome
unsigned int hash_function(const char *name) {
    unsigned int hash = 0;
    while (*name) {
        hash += (unsigned char)(*name++);
    }
    return hash % TABLE_SIZE;
}

// Adiciona um novo contato
void adicionarContato(const char *name, const char *phone) {
    clock_t start = clock();
    
    unsigned int index = hash_function(name);
    Contact *new_contact = (Contact *)malloc(sizeof(Contact));
    strcpy(new_contact->name, name);
    strcpy(new_contact->phone, phone);
    new_contact->next = hash_table[index];
    hash_table[index] = new_contact;

    clock_t end = clock();
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    total_insert_time += time_spent;
    insert_count++;

    printf("Contato adicionado com sucesso. (tempo de inserção: %.2f ms)\n", time_spent);
}

// Buscar um contato pelo nome
void buscarContato(const char *name) {
    clock_t start = clock();
    
    unsigned int index = hash_function(name);
    Contact *current = hash_table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            clock_t end = clock();
            double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

            total_search_time += time_spent;
            search_count++;

            printf("Telefone de %s: %s (tempo de busca: %.2f ms)\n", name, current->phone, time_spent);
            return;
        }
        current = current->next;
    }

    clock_t end = clock();
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    total_search_time += time_spent;
    search_count++;

    printf("Erro: Contato não encontrado. (tempo de busca: %.2f ms)\n", time_spent);
}

// Remove um contato pelo nome
void removerContato(const char *name) {
    unsigned int index = hash_function(name);
    Contact *current = hash_table[index];
    Contact *prev = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                hash_table[index] = current->next;
            }
            free(current);
            printf("Contato removido com sucesso.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Erro: Contato não encontrado.\n");
}

// Mostra todos os contatos
void exibirContatos() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact *current = hash_table[i];
        while (current) {
            printf("Nome: %s, Telefone: %s\n", current->name, current->phone);
            current = current->next;
        }
    }
}

// Exibe tempos de desempenho das funçoes
void metricasDesempenho() {
    double average_insert_time = (insert_count > 0) ? total_insert_time / insert_count : 0;
    double average_search_time = (search_count > 0) ? total_search_time / search_count : 0;

    printf("\nMétricas de desempenho:\n");
    printf("Tempo médio de inserção: %.2f ms\n", average_insert_time);
    printf("Tempo médio de busca: %.2f ms\n", average_search_time);
}

// Menu principal
void menu() {
    int option;
    char name[100], phone[20];

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("5 - Exibir métricas de desempenho\n");
        printf("0 - Sair\n");
        printf("Digite uma opção: ");
        scanf("%d", &option);

        switch (option) {
        case 1:
            printf("Nome: ");
            scanf("%s", name);
            printf("Telefone: ");
            scanf("%s", phone);
            adicionarContato(name, phone);
            break;
        case 2:
            printf("Nome: ");
            scanf("%s", name);
            buscarContato(name);
            break;
        case 3:
            printf("Nome: ");
            scanf("%s", name);
            removerContato(name);
            break;
        case 4:
            exibirContatos();
            break;
        case 5:
            metricasDesempenho();
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 0);
}

int main() {
    // Inicializa a tabela hash com NULL
    memset(hash_table, 0, sizeof(hash_table));

    menu();

    // Libera a memória alocada
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact *current = hash_table[i];
        while (current) {
            Contact *to_free = current;
            current = current->next;
            free(to_free);
        }
    }

    return 0;
}

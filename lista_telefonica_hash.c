#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 100

typedef struct PhoneNode {
    char phone[20];
    struct PhoneNode *next; 
} PhoneNode;

typedef struct Contact {
    char name[50];
    PhoneNode *phones;
    struct Contact *next;
} Contact;

typedef struct HashTable {
    Contact *table[TABLE_SIZE];
} HashTable;

unsigned int hashFunction(const char *name) {
    unsigned int hash = 0;
    while (*name) {
        hash += *name;
        name++;
    }
    return hash % TABLE_SIZE;
}

HashTable *createHashTable() {
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

void addContact(HashTable *hashTable, const char *name, const char *phone) {
    unsigned int index = hashFunction(name);
    Contact *current = hashTable->table[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            PhoneNode *newPhone = (PhoneNode *)malloc(sizeof(PhoneNode));
            strcpy(newPhone->phone, phone);
            newPhone->next = current->phones;
            current->phones = newPhone;
            printf("Número adicionado ao contato existente.\n");
            return;
        }
        current = current->next;
    }

    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strcpy(newContact->name, name);

    PhoneNode *newPhone = (PhoneNode *)malloc(sizeof(PhoneNode));
    strcpy(newPhone->phone, phone);
    newPhone->next = NULL;

    newContact->phones = newPhone;
    newContact->next = hashTable->table[index];
    hashTable->table[index] = newContact;

    printf("Contato adicionado com sucesso!\n");
}

void displayPhones(PhoneNode *phones) {
    while (phones != NULL) {
        printf("  Telefone: %s\n", phones->phone);
        phones = phones->next;
    }
}

void searchContact(HashTable *hashTable, const char *name) {
    unsigned int index = hashFunction(name);
    Contact *current = hashTable->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Contatos de %s:\n", name);
            displayPhones(current->phones);
            return;
        }
        current = current->next;
    }
    printf("Contato não encontrado.\n");
}

void removeContact(HashTable *hashTable, const char *name) {
    unsigned int index = hashFunction(name);
    Contact *current = hashTable->table[index];
    Contact *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            PhoneNode *phone = current->phones;
            while (phone != NULL) {
                PhoneNode *toDelete = phone;
                phone = phone->next;
                free(toDelete);
            }

            if (prev == NULL) {
                hashTable->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Contato removido com sucesso!\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Contato não encontrado.\n");
}

void displayContacts(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact *current = hashTable->table[i];
        if (current != NULL) {
            printf("Índice %d:\n", i);
            while (current != NULL) {
                printf("  Nome: %s\n", current->name);
                displayPhones(current->phones);
                current = current->next;
            }
        }
    }
}

void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact *current = hashTable->table[i];
        while (current != NULL) {
            Contact *toDelete = current;

            PhoneNode *phone = current->phones;
            while (phone != NULL) {
                PhoneNode *toDeletePhone = phone;
                phone = phone->next;
                free(toDeletePhone);
            }

            current = current->next;
            free(toDelete);
        }
    }
    free(hashTable);
}

int main() {
    HashTable *hashTable = createHashTable();
    int choice;
    char name[50];
    char phone[20];

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Nome: ");
                scanf("%s", name);
                printf("Telefone: ");
                scanf("%s", phone);
                addContact(hashTable, name, phone);
                break;
            case 2:
                printf("Nome: ");
                scanf("%s", name);
                searchContact(hashTable, name);
                break;
            case 3:
                printf("Nome: ");
                scanf("%s", name);
                removeContact(hashTable, name);
                break;
            case 4:
                displayContacts(hashTable);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 0);

    freeHashTable(hashTable);
    return 0;
}

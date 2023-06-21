#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABEL_SIZE 20

// Função para obter a label com base no número
char* obterLabel(int numero) {
    FILE* arquivo = fopen("IrisLabels.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo IrisLabels.csv\n");
        exit(1);
    }

    char linha[MAX_LABEL_SIZE];
    int indice = 0;
    while (fgets(linha, MAX_LABEL_SIZE, arquivo) != NULL) {
        if (indice == numero) {
            // Remover a quebra de linha no final
            linha[strcspn(linha, "\n")] = '\0';

            // Alocar memória para a label e copiar a string
            char* label = (char*)malloc(strlen(linha) + 1);
            strcpy(label, linha);

            fclose(arquivo);
            return label;
        }
        indice++;
    }

    fclose(arquivo);
    return NULL; // Número inválido
}

int main() {
    int numGrafos = 3; // Número de grafos
    int i;

    for (i = 1; i <= numGrafos; i++) {
        char nomeArquivo[15];
        sprintf(nomeArquivo, "grafo%d.csv", i);

        FILE* arquivo = fopen(nomeArquivo, "r");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
            exit(1);
        }

        printf("Grafo %d\n", i);

        char linha[100];
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            char* token = strtok(linha, " ");
            while (token != NULL) {
                int numero = atoi(token);

                // Obter a label com base no número
                char* label = obterLabel(numero - 1);  // Ajuste do índice

                if (label != NULL) {
                    printf("%s ", label);
                    free(label);
                } else {
                    printf("Número inválido. ");
                }

                token = strtok(NULL, " ");
            }
            printf("\n");
        }

        fclose(arquivo);
        printf("\n");
    }

    return 0;
}

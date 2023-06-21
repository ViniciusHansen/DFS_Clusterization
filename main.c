#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Lista.c"
#include <string.h>

#define MAX_NODES 150

// Fun  o DFS recursiva
void dfs(int graph[MAX_NODES][MAX_NODES], int node, bool visited[MAX_NODES], int cluster[MAX_NODES], int *clusterSize) {
    visited[node] = true;
    cluster[(*clusterSize)++] = node;

    for (int neighbor = 0; neighbor < MAX_NODES; ++neighbor) {
        if (graph[node][neighbor] && !visited[neighbor]) {
            dfs(graph, neighbor, visited, cluster, clusterSize);
        }
    }
}

// Fun  o para clusteriza  o utilizando DFS
int dfsCluster_verifica(int graph[MAX_NODES][MAX_NODES], int numNodes) {
    bool visited[MAX_NODES] = {false};
    int clusters[MAX_NODES][MAX_NODES];
    int numClusters = 0;
    int validClusters = 0;

    for (int node = 0; node < numNodes; ++node) {
        if (!visited[node]) {
            int clusterSize = 0;
            dfs(graph, node, visited, clusters[numClusters], &clusterSize);

            if (clusterSize <= 50 && clusterSize > 20)
                validClusters ++;

            printf("ClusterSize = %d\n", clusterSize);
            printf("Cluster %d: ", numClusters + 1);
            for (int i = 0; i < clusterSize; ++i) {
                printf("%d ", clusters[numClusters][i]); 
            }
            printf("\n\n");

            numClusters++;
        }
    }
    printf("ValidClusters = %d\n", validClusters);

    if (validClusters > 2)
        return 1;
    else
        return 0;
}

// Função para clusterização utilizando dsf que salva os clusters em listas encadeadas
void dfsCluster_salva(int graph[MAX_NODES][MAX_NODES], int numNodes, Lista *l) {
    bool visited[MAX_NODES] = {false};
    int clusters[MAX_NODES][MAX_NODES];
    int numClusters = 0;
    int vetor_soltos[150], count = 0;
    Lista sub;

    for (int node = 0; node < numNodes; ++node) {
        if (!visited[node]) {
            int clusterSize = 0;
            dfs(graph, node, visited, clusters[numClusters], &clusterSize);

            if (clusterSize <= 50 && clusterSize > 20) {
                inicializa_lista(&sub, sizeof(int));

                for (int i = 0; i < clusterSize; ++i) {
                    insere_fim(&sub, &clusters[numClusters][i]);
                }
                insere_fim(l, &sub);
            }
            else {
                for (int i = 0; i < clusterSize; ++i) {
                    vetor_soltos[count] = clusters[numClusters][i];
                    count++;
                }
            }
            numClusters++;
        }
    }

    inicializa_lista(&sub, sizeof(int));
    for (int i = 0; i < count; ++i) {
        insere_fim(&sub, &vetor_soltos[i]);
    }
    insere_fim(l, &sub);
}


float distanciaEuclidiana(float ponto1[], float ponto2[]) {
    float dx = ponto2[0] - ponto1[0];
    float dy = ponto2[1] - ponto1[1];
    float dz = ponto2[2] - ponto1[2];
    float dw = ponto2[3] - ponto1[3];
    float somaQuadrados = dx*dx + dy*dy + dz*dz + dw*dw;
    float distancia = sqrt(somaQuadrados);
    return distancia;
}

void normalizarMatriz(float matriz[][150], int numLinhas, int numColunas) {
    // Encontra o valor m nimo e m ximo da matriz
    float min = matriz[0][0];
    float max = matriz[0][0];
    for (int i = 0; i < numLinhas; i++) {
        for (int j = 0; j < numColunas; j++) {
            if (matriz[i][j] < min) {
                min = matriz[i][j];
            }
            if (matriz[i][j] > max) {
                max = matriz[i][j];
            }
        }
    }

    // Normaliza cada elemento da matriz
    for (int i = 0; i < numLinhas; i++) {
        for (int j = 0; j < numColunas; j++) {
            float x = matriz[i][j];
            float y = (x - min) / (max - min);
            matriz[i][j] = y;
        }
    }
}

int calcular_centro_grafo(float matriz_adjacencias[MAX_NODES][MAX_NODES]) {
    int centro = -1;
    int soma_minima = MAX_NODES * MAX_NODES; // Valor máximo inicial
    
    // Calcular a soma das distâncias para cada vértice
    for (int i = 0; i < MAX_NODES; i++) {
        int soma_distancias = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            soma_distancias += matriz_adjacencias[i][j];
        }
        
        // Verificar se a soma é menor que a soma mínima atual
        if (soma_distancias < soma_minima) {
            soma_minima = soma_distancias;
            centro = i;
        }
    }
    
    return centro;
}

float calcular_distancia(int v1, int v2, float matriz_distancias[150][150]) {
    return matriz_distancias[v1][v2];
}

float calcular_distancia_ponto_centro_cluster(int ponto, Lista cluster, float matriz_distancias[150][150]) {
    float distancia = 0.0;
    int contador = 0;
    
    int atual;

    for (int i = 0 ; i < 60; i++) {
        if( le_valor(cluster, &atual , i) == ERRO_POS_INVALIDA)
            break;

        distancia += calcular_distancia(ponto, atual, matriz_distancias);
        contador++;
                
    }

    if (contador > 0) {
        distancia /= contador;
    }
    
    return distancia;
}


char menor_valor(float a, float b, float c) {
    char nome_variavel = 'A';
    float menor = a;
    
    if (b < menor) {
        menor = b;
        nome_variavel = 'B';
    }
    
    if (c < menor) {
        menor = c;
        nome_variavel = 'C';
    }
    
    return nome_variavel;
}

int main()
{
    FILE *arquivo, *lista_adjacencias, *arq_labels;
    int i, j;
    char buffer[200];
    float matriz_pontos[150][4], matriz_distancia[150][150], matriz_normalizada[150][150];

    arquivo = fopen("IrisDataset.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!");
        exit(1);
    }


    FILE *output;
    output = fopen("output.csv", "w"); // Abre o arquivo em modo de escrita ("w")
    
    if (output == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }


    // ler matriz pontos
    fscanf(arquivo, "%s", buffer);

    for (i = 0; i < 150; i++) {
        for (j = 0; j < 4; j++) {
            fscanf(arquivo, "%f", &matriz_pontos[i][j]);
            if (j != 4 - 1) {
                fscanf(arquivo, ",");
            }
        }
    }

    fclose(arquivo);


    //ler labels
    /*
    arq_labels = fopen("IrisLabels.csv", "r");
    if (arq_labels == NULL) {
        printf("Erro ao abrir o arquivo!");
        exit(1);
    }

    char labels[150][20];
    for(int i=0;i<150;i++){
        fscanf(arq_labels, "%s \n", labels[i]);
    }
    */

    // printar matriz pontos
    printf("Matriz de Pontos:\n");
    for (i=0;i<150;i++){
        for (j=0;j<4;j++){
            printf("%.2f\t", matriz_pontos[i][j]);
        }
        printf("\n");
    }

    // preencher matriz_distancia
    for (int i = 0; i < 150; i++) {
        for (int j = 0; j < 150; j++) {
            float distancia = distanciaEuclidiana(matriz_pontos[i], matriz_pontos[j]);
            matriz_distancia[i][j] = distancia;
        }
    }


    // printar uma parte da matriz_distancias

    printf("Parte da matriz de Dist ncias:\n");
    for (i=0;i<20;i++){
        for (j=0;j<i;j++){
            printf("%.2f\t", matriz_distancia[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < 150; i++) {
        for (int j = 0; j < 150; j++) {
            matriz_normalizada[i][j] = matriz_distancia[i][j];
        }
    }

    // Normalizar Matriz
    normalizarMatriz(matriz_normalizada,150,150);

    // printar uma parte da matriz_distancias

    printf("Parte da matriz de Dist ncias Normalizada:\n");
    for (i=0;i<20;i++){
        for (j=0;j<i;j++){
            printf("%.2f\t", matriz_normalizada[i][j]);
        }
        printf("\n");
    }

    // para todo o par i e j que for menor que 0.3 fazer lista de adjacencias, começando em 0,002, iterando a cada +0,002

    float c = 0.002, vetor_limiares_validos[10];
    int ci = 0;
    Lista grafos;
    inicializa_lista(&grafos, sizeof(Lista));
    for (int k =0; k < 150; k ++) {
        printf("\n c = %f \n", c);
        int matriz_adjacencias[150][150] = {0};
        for (i=0;i<150;i++){
            for (j=0;j<150;j++){
                if (matriz_normalizada[i][j] <= c)
                    matriz_adjacencias[i][j] = 1;
                else
                    matriz_adjacencias[i][j] = 0;
            }

        }

        if ( dfsCluster_verifica(matriz_adjacencias,150) == 1) {
            vetor_limiares_validos[ci] = c;
            dfsCluster_salva(matriz_adjacencias,150, &grafos);
            ci++;
        }

        c += 0.002;

        printf("\n---------\n");
    }

    //printa os limiares de distância em que uma boa clusterização foi encontrada
    printf("Limiares bons:\n");
    for ( int k = 0; k < ci; k++) {
        printf("%f\n", vetor_limiares_validos[k]);

    }

    Lista sub;
    Lista cluster1;
    Lista cluster2;
    Lista cluster3;
    Lista sobras;
    Lista teste;
    int valor;
    inicializa_lista(&sub, sizeof(int));
    inicializa_lista(&cluster1, sizeof(int));
    inicializa_lista(&cluster2, sizeof(int));
    inicializa_lista(&cluster3, sizeof(int));
    inicializa_lista(&sobras, sizeof(int));

    // 1 iteração sobre o grafo (1 limiar)
    for( i = 0; i < ci*4; i+=4) {
        //le_valor(grafos, &sub, i); // Lê um grafo
        le_valor(grafos, &cluster1, 0+i); // 4  8
        le_valor(grafos, &cluster2, 1+i); // 5  9
        le_valor(grafos, &cluster3, 2+i); // 6  10
        le_valor(grafos, &sobras, 3+i);   // 7  11


        printf("\nGrafo %d\n", i/4 + 1);
        //printf("Centro: %i\n", calcular_centro_grafo(matriz_distancia));

        //printf("\n\nSobras\n");
            for (j = 0 ; j < 60; j++) {
                if( le_valor(sobras, &valor, j) == ERRO_POS_INVALIDA)
                    break;

                //printf("Valor sobrando: %4d  ", valor);

                float A = calcular_distancia_ponto_centro_cluster(valor, cluster1, matriz_distancia);
                float B = calcular_distancia_ponto_centro_cluster(valor, cluster2, matriz_distancia);
                float C = calcular_distancia_ponto_centro_cluster(valor, cluster3, matriz_distancia);
                
                char menor = menor_valor(A,B,C);

                switch (menor) {
                    case 'A':
                        insere_fim(&cluster1,&valor);
                        break;
                    case 'B':
                        insere_fim(&cluster2,&valor);
                        break;
                    case 'C':
                        insere_fim(&cluster3,&valor);
                        break;
                }

                //printf("\nmenor valor : %c\n",menor);
                //printf("Distancia do valor: %4d para\ncluster1: %f\ncluster2: %f\ncluster3: %f\n", valor, A, B, C); 
            }
            printf("\n\nCluster 1\n");
            for (j = 0 ; j < 100; j++) {
                if( le_valor(cluster1, &valor, j) == ERRO_POS_INVALIDA)
                    break;

                printf("%4d", valor);
            }
            printf("\n\nCluster 2\n");
            for (j = 0 ; j < 100; j++) {
                if( le_valor(cluster2, &valor, j) == ERRO_POS_INVALIDA)
                    break;

                printf("%4d", valor);
            }
            printf("\n\nCluster 3\n");
            for (j = 0 ; j < 100; j++) {
                if( le_valor(cluster3, &valor, j) == ERRO_POS_INVALIDA)
                    break;

                printf("%4d", valor);
            }
        printf("\n\n");
    }






    for (i = 0 ; i < 25; i++) {
        le_valor(grafos, &sub, i);
        limpa_lista(&sub);
    }
    limpa_lista(&grafos);



    return 0;
}

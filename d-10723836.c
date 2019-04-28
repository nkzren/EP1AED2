/*************************************
 *   EP1 - AED 2 - 2019_1            *
 *   Prof: Helton Hideraldo Biscaro  *
 *   Aluno: Renan Nakazawa           *
 *   NUSP: 10723836                  *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define ARESTA_NULA -1
#define VERTICE_INVALIDO -1
#define MAX_VERTICES 50
#define infinito INT_MAX

typedef int Peso;
typedef struct {
    Peso matriz[MAX_VERTICES + 1][MAX_VERTICES + 1];
    int numVertices;
    int numArestas;
} Grafo;

typedef struct {
    int elementoPai;
    Peso peso;
    bool visitado;
} Elemento;
typedef Elemento* PontElemento;

typedef struct {
    PontElemento* elementos;
    int custoMinimo;
    int numElementos;
} Arvore;


/* Declaração de funções*/
Grafo* initGrafo(int vertices, int arestas);
Arvore* initArvore(int numElementos);

void split(char linha[], int array[]);
int chaveMinima(Arvore *arvore);

Arvore* prim(Grafo* grafo, char (*string)[20], FILE* file);
PontElemento* ordenaElementos(Arvore* arvore);

void imprimeMatrizAdj(Grafo *grafo);
void imprimeArvore(Arvore* arvore, char (*stringSaida)[], FILE* file);

int main (int argc, char *argv[]) {
    char *entrada = argv[1];
    char *saida = argv[2];

    // Abre o arquivo e verifica se foi aberto corretamente
    FILE *file = fopen(entrada, "r");
    if(NULL == file) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", entrada);
        return 1;
    }

    // Define um tamanho inicial para as linhas
    size_t buffer_size = 50;
    char *buffer = malloc(buffer_size * sizeof(char));

    // Lê a primeira linha para inicializar o grafo
    getline(&buffer, &buffer_size, file);
    int array[2];
    split(buffer, array);

    Grafo *grafo = initGrafo(array[0], array[1]);

    // Lê o restante das linhas
    int arestas[grafo->numVertices];
    while(-1 != getline(&buffer, &buffer_size, file)) {
        split(buffer, arestas);
        grafo->matriz[arestas[0]][arestas[1]] = arestas[2];
        grafo->matriz[arestas[1]][arestas[0]] = arestas[2];
    }
    /**
     * Descomentar a linha abaixo para imprimir
     * a matriz de adjacência do grafo no console 
     */
    // imprimeMatrizAdj(grafo);

    char stringSaida[grafo->numVertices][20];
    FILE* fileOutput = fopen(saida, "w");
    Arvore* mst = prim(grafo, stringSaida, fileOutput);

    fflush(stdout);

    // Fecha o arquivo e desaloca a memória do buffer
    fclose(file);
    free(buffer);

    return 0;
}

Arvore* prim(Grafo* grafo, char (*stringSaida)[20], FILE* file) {
    Arvore* mst = initArvore(grafo->numVertices);
    
    // Inicia a busca pelo vértice 0
    mst->elementos[0]->peso = 0;
    mst->elementos[0]->elementoPai = -1;

    for (int count = 0; count < grafo->numVertices; count++) {
        // Selecionar o menor custo dentre as arestas visitadas
        int i = chaveMinima(mst);
        mst->elementos[i]->visitado = true;
        mst->custoMinimo += grafo->matriz[mst->elementos[i]->elementoPai][i];
        // Atualiza o valor da peso e o pai de cada elemento na MST
        for (int j = 0; j < mst->numElementos; j++) {
            /*
             * Cond. 1: A matriz do grafo só é 0 se há uma aresta entre os vértices i e j
             * Cond. 2: Atualiza somente os não visitados
             * Cond. 3: Atualiza somente se a aresta for menor que a peso
             */
            if (grafo->matriz[i][j] != ARESTA_NULA
                && mst->elementos[j]->visitado == false
                && grafo->matriz[i][j] < mst->elementos[j]->peso) {
                    mst->elementos[j]->elementoPai = i;
                    mst->elementos[j]->peso = grafo->matriz[i][j];
                }
        }
        sprintf(stringSaida[count], "%d %d\n", mst->elementos[i]->elementoPai, i);
    }
    imprimeArvore(mst, stringSaida, file);

    return mst;
}

/************************
 *                      *
 * FUNÇÕES AUXILIARES   *
 *                      *
 ************************/

// Separa os argumentos de cada linha e armazena num array de int
void split(char linha[], int array[]) {
    char splitters[] = " ""\r""\n";
    char *ponteiro = strtok(linha, splitters);
    int qtdArgs = 0;
    while (ponteiro != NULL) {
        array[qtdArgs++] = atoi(ponteiro);
        ponteiro = strtok(NULL, " ");
    }
}

// Inicializa todos os valores do grafo com custo máximo
Grafo* initGrafo(int vertices, int arestas) {
    if (vertices <= 0 || arestas < 0) {
        return NULL;
    }
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->numVertices = vertices;
    grafo->numArestas = arestas;
    int i;
    for (i = 0; i < vertices; i++) {
        int j;
        for (j = 0; j < vertices; j++) {
            grafo->matriz[i][j] = ARESTA_NULA;
        }
    }
    return grafo;
}

Arvore* initArvore(int numElementos) {
    if (numElementos <= 0) 
        return NULL;

    Arvore* arvore = (Arvore*) malloc (sizeof(Arvore));
    arvore->elementos = (PontElemento*) malloc(numElementos * sizeof(PontElemento));
    arvore->numElementos = numElementos;
    arvore->custoMinimo = 0;

    for (int i = 0; i < numElementos; i++) {
        arvore->elementos[i] = (PontElemento) malloc (sizeof(Elemento));
        arvore->elementos[i]->elementoPai = -1;
        arvore->elementos[i]->peso = infinito;
        arvore->elementos[i]->visitado = false;
    }
    return arvore;
}

// Encontra o menor dentre os pesos adjacentes do vértice
int chaveMinima(Arvore* arvore) {
    int valorMinimo = infinito;
    int indexMinimo;

    for (int i = 0; i < arvore->numElementos; i++) {
        PontElemento* elementos = arvore->elementos;
        if (elementos[i]->visitado == false && elementos[i]->peso < valorMinimo) {
            valorMinimo = elementos[i]->peso;
            indexMinimo = i;
        }
    }
    return indexMinimo;
}

// Imprime a matriz de adjacencia do grafo no arquivo de entrada
void imprimeMatrizAdj(Grafo *grafo) {
    int i,j;
    for (i = 0; i < grafo->numVertices; i++) {
        printf("\n");
        for (j = 0; j < grafo->numVertices; j++) {
            printf ("%i\t", grafo->matriz[i][j]);
        }
    }
    printf("\n");
}

void imprimeArvore(Arvore* arvore, char (*string)[20], FILE* file) {
    fprintf(file, "%i\n", arvore->custoMinimo);
    for (int i = 1; i < arvore->numElementos; i++) {
        fprintf(file, "%s", string[i]);
    }
    fclose(file);
}

PontElemento* ordenaElementos(Arvore* arvore) {
    printf("Inicio ordenação\n");
    int pai = 0;
    PontElemento aux = (PontElemento) malloc(sizeof(Elemento));
    for (int i = 0; i < arvore->numElementos; i++) {
        for (int j = 0; j < arvore->numElementos; j++) {
            if (arvore->elementos[j]->elementoPai == pai) {
                pai = j;
                aux = arvore->elementos[i];
                arvore->elementos[i] = arvore->elementos[j];
                arvore->elementos[j] = aux;
                break;
            }
        }
    }
    return NULL;
}

/* ----------------------------------------------------------- */
/*               EP1 - AED II - 1o semestre 2019               */
/*               Prof. Helton Hideraldo Biscaro                */
/*      Aluno: Raphael Janofsky Pivato   No USP: 10851026      */
/* ----------------------------------------------------------- */

/* --------------------- DEFINICAO DE CONSTANTES E INCLUSAO DAS BIBLIOTECAS -------------------- */

	#include <stdlib.h>
	#include <stdio.h>
	#include <stdbool.h>			
	#include <malloc.h>
		
	#define INFINITO 999999999
	#define NULO -1
	
/* --------------------- DEFINICAO DE TIPOS E ESTRUTURAS --------------------- */

	typedef int TipoPeso;

	typedef struct taresta{
		int vdest;
		TipoPeso peso;
		struct taresta* prox;
	} TipoAresta;

	typedef TipoAresta* TipoApontador;
	
	typedef struct{
		TipoApontador* listaAdj;
		int numVertices;
		int numArestas;
	} TipoGrafo;
	
	typedef struct aux{
		int chave;
		int antecessor;
		int vertice;
		struct aux* prox;
	} ROTULO, * PONT;
	
	typedef struct{
		PONT cabeca;
	} LISTADEROTULOS, * PLISTA;
	
/* --------------------- METODOS --------------------- */

	/* Cria um grafo com nv vertices, aloca espaco para o vetor de apontadores de listas de adjacencias e, para cada vertice, 
		inicializa o apontador de sua lista de adjacencia. Retorna true se inicializou com sucesso e false caso contrario.
		Vertices vao de 1 a nv */
		
	TipoGrafo* inicializaGrafo(int nv){
		if (nv <= 0)
			return NULL;
	
		TipoGrafo* grafo = (TipoGrafo*) malloc (sizeof (TipoGrafo));
		grafo -> listaAdj = (TipoApontador*) malloc (nv * sizeof (TipoApontador));
		grafo -> numVertices = nv;
		grafo -> numArestas = 0;
		
		int i;
		
		for (i = 0; i < nv; i++){
			grafo -> listaAdj[i] = (TipoApontador) malloc (sizeof (TipoAresta));
			grafo -> listaAdj[i] -> prox = NULL;
		}
		
		return grafo;
	}
	
	/* Insere de modo NAO ORDENADO a aresta (v1, v2) com peso "peso" no grafo. Nao verifica se a aresta ja existe. 
		Como o grafo e nao direcionado, insere tambem a aresta (v2, v1). */
	
	void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo* grafo){
		if (v1 >= 0 && v1 < grafo -> numVertices && v2 >= 0 && v2 < grafo -> numVertices && grafo != NULL){
			
			TipoAresta* arestaV1V2 = (TipoAresta*) malloc (sizeof (TipoAresta));
			TipoAresta* arestaV2V1 = (TipoAresta*) malloc (sizeof (TipoAresta));
			
			arestaV1V2 -> vdest = v2;
			arestaV1V2 -> peso = peso;
			arestaV1V2 -> prox = grafo -> listaAdj[v1] -> prox;
			arestaV2V1 -> vdest = v1;
			arestaV2V1 -> peso = peso;
			arestaV2V1 -> prox = grafo -> listaAdj[v2] -> prox;
			
			grafo -> listaAdj[v1] -> prox = arestaV1V2;
			grafo -> listaAdj[v2] -> prox = arestaV2V1;
			grafo -> numArestas++;
		}
	}
	
	/* Cria um lista de rotulos para os vertices de um grafo, alocando memoria para a lista e para o no cabeca, sendo
		este ultimo inicializado com chave de valor INFINITO, vertice atual e antecessor com valor NULO; e prox apontando
		para NULL */
	
	PLISTA criarListaDeRotulos(){
		PLISTA resp = (PLISTA) malloc (sizeof (LISTADEROTULOS));
		resp -> cabeca = NULL;
		
		return resp;
	}
	
	/* Recebe uma lista de rotulos, uma chave, um vertice antecessor e um vertice atual como parametros. Na lista, aloca
		memoria para um novo rotulo e atribui a ele os valores passados por parametro. Nao ha verificacao de insercao
		de vertices iguais e o novo elemento e sempre inserido no inicio, apos o no cabeca */
	
	void insereRotulo(PLISTA listaDeRotulos, int chave, int antecessor, int vertice){
		if (listaDeRotulos != NULL && chave >= 0 && antecessor >= NULO){
			PONT novo = (PONT) malloc (sizeof (ROTULO));
			novo -> chave = chave;
			novo -> antecessor = antecessor;
			novo -> vertice = vertice;
			novo -> prox = listaDeRotulos -> cabeca;
			
			listaDeRotulos -> cabeca = novo;
		}
	}
	
	/* Libera o espaco ocupado por um rotulo na lista de rotulos, realizando o acerto dos ponteiros necessarios */
	
	void removeRotulo(PLISTA listaDeRotulos, int vertice){
		PONT atual, anterior;
		atual = listaDeRotulos -> cabeca;
		anterior = (PONT) &listaDeRotulos -> cabeca;
		
		while (atual){
			if (atual -> vertice == vertice)
				break;
			
			anterior = atual;
			atual = atual -> prox;
		}
		
		if (atual != NULL){
		  if (anterior == (PONT) &listaDeRotulos -> cabeca)
			listaDeRotulos -> cabeca = atual -> prox;
		
		  else
			anterior -> prox = atual -> prox;
		
		  free(atual);
		}
	}
	
	/* Retorna o ponteiro para o elemento do vertice buscado na lista de rotulos ou NULL caso tenha percorrido a lista toda
		e nao encontrar igualdade */
	
	PONT buscaVerticeLista(PLISTA listaDeRotulos, int vertice){
		PONT atual = listaDeRotulos -> cabeca;
				
		while (atual){
			if (atual -> vertice == vertice)
				return atual;
			
			atual = atual -> prox;	
		}
		
		return atual;
	}
	
	/* Percorre a lista para identificar a menor chave presente, retornando o ponteiro para tal elemento */
	
	PONT minimoLista(PLISTA listaDeRotulos){
		if (listaDeRotulos != NULL && listaDeRotulos -> cabeca != NULL){
			PONT menorElemento = listaDeRotulos -> cabeca;
			PONT atual = menorElemento -> prox;
			
			while (atual){
				if (atual -> chave < menorElemento -> chave)
					menorElemento = atual;
				
				atual = atual -> prox;
			}
		
			return menorElemento;
		}
		
		return NULL;
	}
	
	/* Insere a aresta segura (v1, v2) em um grafo do tipo AGM. Este metodo nao considera inserir a aresta (v2, v1) a fim
		de facilitar sua posterior impressao em arquivo de saida */	

	void insereArestaSegura(TipoGrafo* agm, TipoPeso peso, int v1, int v2){
		if (v1 >= 0 && v1 < agm -> numVertices && v2 >= 0 && v2 < agm -> numVertices && agm != NULL){
			
			TipoApontador arestaV1V2 = (TipoApontador) malloc (sizeof (TipoAresta));
			
			arestaV1V2 -> vdest = v2;
			arestaV1V2 -> peso = peso;
			arestaV1V2 -> prox = agm -> listaAdj[v1] -> prox;
		
			agm -> listaAdj[v1] -> prox = arestaV1V2;
			agm -> numArestas++;
		}
	}
	
	/* -------------------------------- ALGORITMO DE PRIM ---------------------------------------- */

	/* Cria uma lista de rotulos para os vertices do grafo recebido como parametro, inicializando cada rotulo com chave 
	INFINITO e antecessor NULO. 
	Para gerir as informacoes entre as diferentes estruturas, tres ponteiros sao criados:
		1) raizAtual que fica travado no elemento raiz da lista e que e atualizado a cada iteracao representando sempre 
			o elemento de menor chave;
		2) atualGrafo que percorre as listas de adjacencia de cada vertice do grafo;
		3) atualLista que percorre a lista de rotulos a fim de atualizar cada elemento conforme a necessidade.
	Um grafo do tipo AGM e entao criado e inicializado com a mesma quantidade de vertices do grafo passado por parametro.
	Na sequencia, i recebera o valor do vertice do elemento menos custoso e a lista de adjacencia desse vertice sera 
		percorrida em sua totalidade, a fim de atualizar a chave de cada elemento compativel, presente na lista de rotulos 
		e que possuia custo superior ao da leitura atual.
	Finalizando a iteracao, o menor elemento (raiz) e descartado da lista e o processo se repetira enquanto a lista de rotulos
		possuir elementos validos,, garantindo que a AGM representara o sub-grafo com o caminho menos custoso e que conecte 
		todos os vertices do grafo original, sendo essa retornada a quem chamou o metodo */
	
	TipoGrafo* arvoreGeradoraMinima(TipoGrafo* grafo, int raiz){
		if (grafo != NULL && raiz >= 0 && raiz < grafo -> numVertices){
			
			PLISTA lRotulos = criarListaDeRotulos();
			
			int i;
			for (i = 0; i < grafo -> numVertices; i++)
				insereRotulo(lRotulos, INFINITO, NULO, i); 
			
			PONT raizAtual, atualLista;
			TipoApontador atualGrafo;
			
			TipoGrafo* agm = inicializaGrafo(grafo -> numVertices); 
			
			raizAtual = buscaVerticeLista(lRotulos, raiz); 
			raizAtual -> chave = 0;
			
			while (lRotulos -> cabeca){
				i = raizAtual -> vertice;
				atualGrafo = grafo -> listaAdj[i] -> prox;
				
				while (atualGrafo){
					atualLista = buscaVerticeLista(lRotulos, atualGrafo -> vdest); 
					
					if (atualLista && atualGrafo -> peso < atualLista -> chave){
						atualLista -> antecessor = i;
						atualLista -> chave = atualGrafo -> peso;
					}
					
					atualGrafo = atualGrafo -> prox;
				}
				
				insereArestaSegura(agm, raizAtual -> chave, raizAtual -> antecessor, raizAtual -> vertice);
				/* */
				printf("%d  %d\n", raizAtual -> antecessor, raizAtual -> vertice);
				
				removeRotulo(lRotulos, raizAtual -> vertice);  
				
				raizAtual = minimoLista(lRotulos); 
			}
		
			return agm;
		}
		
		return NULL;
	}
	
	/* Recebe uma AGM e retorna a soma de todos os pesos de suas arestas */
	
	int pesoTotalAGM(TipoGrafo* agm){
		int soma = 0;
		int i;
		TipoApontador atual;
		
		for (i = 0; i < agm -> numVertices; i++){
			atual = agm -> listaAdj[i] -> prox;
			
			while (atual){
				soma += atual -> peso;
				atual = atual -> prox;
			}
		}
		
		return soma;
	}
	
	/* Realiza a gravacao do arquivo de saida, imprimindo um cabecalho com a somatoria dos pesos de todas as arestas e, em
		sequencia, cada linha representa uma aresta no formato (v1 v2), possibilitando uma facil representacao grafica
		da AGM */
	
	void gravaArquivoSaida(TipoGrafo* agm, FILE* pont_arq){
		int somatoria = pesoTotalAGM(agm);
		
		fprintf (pont_arq, "%i\n", somatoria);
		
		int i; 
		TipoApontador atual;
		
		for (i = 0; i < agm -> numVertices; i++){
			for (atual = agm -> listaAdj[i] -> prox; atual != NULL; atual = atual -> prox)
				fprintf (pont_arq, "%i %i\n", i, atual -> vdest);
		}
	}
	
	/* Le o arquivo de entrada e abastece um grafo pre-gerado para ser manipulado pelo programa */
	
	void leArquivoEntrada(TipoGrafo* grafo, FILE* pont_arq){
		if (pont_arq != NULL && grafo != NULL){
			
			int entrada[50];
			int i;
			
			for (i = 0; i < 50; i++)
				entrada[i] = NULO;
		
			i = 0;
			
			while (i < 50 && i != EOF){
				fscanf(pont_arq, "%d ", &entrada[i]);
				if (i == 0){
					grafo = inicializaGrafo(entrada[i]);
					i++;
				}
				else if (i == 1)
					i++;
				else{
					insereAresta(entrada[i], entrada[i + 1], entrada[i + 2], grafo);
					i += 3;
				}
			}			
		}
	}
	
	/* -------------------------------- MAIN ---------------------------------------- */
	
	/* Declara um grafo, le o arquivo de entrada para alocar a memoria necessaria conforme o layout estabelecido e, por fim,
		utiliza a implementacao do algoritmo de Prim para produzir a Arvore Geradora Minima do grafo, exibida em um txt de
		saida, conforme layout preestabelecido via pdf "ep1" */
		
	int main(){
		/*FILE* arq_entrada = fopen("grafo01.txt", "r");
		
		if (arq_entrada == NULL){
			printf("ERRO: Falha na abertura do ponteiro para o arquivo de entrada\n");
			
			return 1;
		}
		*/
		
		/* */
		TipoGrafo* grafo = inicializaGrafo(7);
		
		insereAresta(0, 1, 8, grafo);
		insereAresta(0, 2, 2, grafo);
		insereAresta(0, 3, 11, grafo);
		insereAresta(1, 3, 2, grafo);
		insereAresta(1, 4, 4, grafo);
		insereAresta(2, 3, 8, grafo);
		insereAresta(2, 5, 5, grafo);
		insereAresta(3, 5, 13, grafo);
		insereAresta(3, 6, 8, grafo);
		insereAresta(4, 6, 1, grafo);
		insereAresta(5, 6, 3, grafo);
		
		/*
		leArquivoEntrada(grafo, arq_entrada);
		
		FILE* arq_saida = fopen("saidagrafo01.txt", "w");
		
		if (arq_saida == NULL){
			printf("ERRO: Falha na abertura do ponteiro para o arquivo de saida\n");
			
			return 1;
		}
		*/
		TipoGrafo* agm = arvoreGeradoraMinima(grafo, 0);
		
		/* */
		int pt = pesoTotalAGM(agm);
		printf("%d n", pt);
		/*
		gravaArquivoSaida(agm, arq_saida);
		
		fclose(arq_entrada);
		fclose(arq_saida);
		*/
		printf("Acoes concluidas com sucesso!\n");
		
		return 0;
	}
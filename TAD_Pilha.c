#include <stdlib.h>
#include <stdio.h>
#include "TAD_Pilha.h"


int init_TADPilha(TAD_PILHA *p, int tamanho) {

    int i;

    if (tamanho<=0) return 0;
    p->pilha=(float *)malloc(tamanho*sizeof(float));
    if (p->pilha==NULL) return 0;
    for (i=0; i<tamanho; i++) p->pilha[i]=0.0;    // acrescentada funcionalidade de pilha inicializada zerada
    p->TAM=tamanho;
    p->topo=-1;
    return 1;
}

int push_TADPilha(TAD_PILHA *p, float valor) {

    if (p->topo==p->TAM-1) return 0;    // Pilha cheia
    p->topo++;
    p->pilha[p->topo]=valor;
    return 1;
}

int pop_TADPilha(TAD_PILHA *p, float *pValor) {

    if (p->topo==-1) return 0;          // Pilha vazia
    *pValor=p->pilha[p->topo];
    p->topo--;
    return 1;
}

int tam_TADPilha(TAD_PILHA *p) {

    return(p->topo+1);
}

float at_top_TADPilha(TAD_PILHA *p) {

    return(p->pilha[p->topo]);
}

int free_TADPilha(TAD_PILHA *p) {

    if (p->pilha==NULL) return 0;       // pilha nao inicializada (alocada)
    free(p->pilha);
    return 1;
}

void print_TADPilha(TAD_PILHA *p) {

    int i;

    printf("Topo [");
    for (i=p->topo; i>=0; i--)
        printf("%5.2f ", p->pilha[i]);
    printf("] Fim\n");

}

#ifndef TAD_PILHA_H_INCLUDED
#define TAD_PILHA_H_INCLUDED

typedef
struct {
    int TAM;
    int topo;
    float *pilha;
} TAD_PILHA;

/* Protótipos das funcoes  */

int init_TADPilha(TAD_PILHA *, int);
int push_TADPilha(TAD_PILHA *, float);
int pop_TADPilha(TAD_PILHA *, float *);
int free_TADPilha(TAD_PILHA *);
int tam_TADPilha(TAD_PILHA *);
float at_top_TADPilha(TAD_PILHA *);
void print_TADPilha(TAD_PILHA *);

#endif // TAD_PILHA_H_INCLUDED

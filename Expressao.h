#ifndef EXPRESSAO_H_INCLUDED
#define EXPRESSAO_H_INCLUDED

#include <gtk/gtk.h>

#define TAM_MAX_ENTRADA_POR_LINHA 500

#define SOMA        1.0
#define SUBTRAI     2.0
#define MULTIPLICA  3.0
#define DIVIDE      4.0

typedef
enum {OPERANDO, OPERADOR, ABRE_PARENTESES, FECHA_PARENTESES, IGUAL}
CLASSE_OP;

typedef
enum {EXPRESSAO, SUB_EXPRESSAO}
TIPO_EXP;

typedef
struct {
    CLASSE_OP tipo;
    union {
        float codOperador;
        float valOperando;
    };
} TOKEN;

typedef
enum {FALSO=0, VERDADEIRO}
BOOL;

/* Protótipos das funcoes  */

void aborte(char *, GtkWidget *);
TOKEN next_token(const char *, int *, int *, GtkWidget *);
float calcula(TIPO_EXP, const char *, int *, int *, GtkWidget *);
void toPosfix(TIPO_EXP, const char *, int *, int *, GtkWidget *, char *);

#endif // EXPRESSAO_H_INCLUDED

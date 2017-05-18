#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <gtk/gtk.h>
#include "TAD_Pilha.h"
#include "Expressao.h"


void aborte(char *msg, GtkWidget *window) {

    GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_NONE,
				msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "Erro!");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog); 
	
}


TOKEN next_token(const char *expr, int *pIdx, int *erro, GtkWidget *window) {

    TOKEN t;
    char c;
    char lexema[30]="";
    int sinal, cont=0;
    int ocorreuPonto;

    c=expr[(*pIdx)++];
    if (c=='\0') {
      t.tipo=IGUAL;
      return t;
    }
    while (c=='\n' || c==' ' || c=='\t') c=expr[(*pIdx)++];
    if (c=='\0') {
      t.tipo=IGUAL;
      return t;
    }
    else if (isdigit(c) || c=='~') {
        sinal=1;
        while (c=='~') {
            sinal*=-1;
            c=expr[(*pIdx)++];
        }
        if (!isdigit(c)) { aborte("Expressao mal formatada", window); *erro=1; return t; }
        ocorreuPonto=0;
        while (isdigit(c) || c=='.') {
            lexema[cont++]=c;
	    if (c=='.' && ocorreuPonto) { aborte("Expressao mal formatada", window); *erro=1; return t; }
	    if (c=='.') ocorreuPonto=1;
            lexema[cont]='\0';
            c=expr[(*pIdx)++];
        }
        (*pIdx)--;
        t.tipo=OPERANDO;
        t.valOperando=atof(lexema)*sinal;
        return t;
    }
    else if (c=='+' || c=='-' || c=='*' || c=='/') {
        t.tipo=OPERADOR;
        switch (c) {
            case '+': t.codOperador=SOMA; return t;
            case '-': t.codOperador=SUBTRAI; return t;
            case '*': t.codOperador=MULTIPLICA; return t;
            case '/': t.codOperador=DIVIDE; return t;
        }
    }
    else if (c=='=') {
        t.tipo=IGUAL;
        return t;
    }
    else if (c=='(') {
        t.tipo=ABRE_PARENTESES;
        return t;
    }
    else if (c==')') {
        t.tipo=FECHA_PARENTESES;
        return t;
    }
    else { aborte("Expressao mal formatada", window); *erro=1; return t; }
    return t; // apenas para evitar warnings do compilador - "funcao termina sem return"
}

float calcula(TIPO_EXP tipoExp, const char *expr, int *pIdx, int *erro, GtkWidget *window) {

    TOKEN t;
    float x, y;
    float op;
    float resultado=0;
    TAD_PILHA operando, operador;

    init_TADPilha(&operando, 5);
    init_TADPilha(&operador, 5);

    while (1) {
        t=next_token(expr, pIdx, erro, window); // busca o proximo elemento (token) da entrada
		if (*erro) return(0);
        if (t.tipo==ABRE_PARENTESES) {          // o que esta entre parenteses eh uma sub-expressao completa
            resultado=calcula(SUB_EXPRESSAO, expr, pIdx, erro, window);   // e eh tambem um operando da expressao que contem o "()"
			if (*erro) return(0);
            t.tipo=OPERANDO;
            t.valOperando=resultado;
        }
        if (t.tipo==OPERANDO) {
            if (tam_TADPilha(&operando)==0) push_TADPilha(&operando, t.valOperando);
            else if ((at_top_TADPilha(&operador)==MULTIPLICA) || (at_top_TADPilha(&operador)==DIVIDE)) {
                pop_TADPilha(&operando, &x);
                pop_TADPilha(&operador, &op);
                resultado = (op==MULTIPLICA?x*t.valOperando:x/t.valOperando);
                push_TADPilha(&operando, resultado);
            }
            else if ((at_top_TADPilha(&operador)==SOMA) || (at_top_TADPilha(&operador)==SUBTRAI)) {
                push_TADPilha(&operando, t.valOperando);
            }
            else { aborte("Expressao mal formatada", window); *erro=1; return (0); }
        }
        else if (t.tipo==OPERADOR) {
            if (tam_TADPilha(&operando)==0) { aborte("Expressao mal formatada", window); *erro=1; return (0); }
            else if (tam_TADPilha(&operando)==1) push_TADPilha(&operador, t.codOperador);
            else if (tam_TADPilha(&operando)>=2) {
                if ((t.codOperador==MULTIPLICA) || (t.codOperador==DIVIDE))
                    push_TADPilha(&operador, t.codOperador);
                else if ((t.codOperador==SOMA) || (t.codOperador==SUBTRAI)) {
                    pop_TADPilha(&operando, &y);
                    pop_TADPilha(&operando, &x);
                    pop_TADPilha(&operador, &op);
                    resultado = (op==SOMA?x+y:x-y);
                    push_TADPilha(&operando, resultado);
                    push_TADPilha(&operador, t.codOperador);
                }
                else { aborte("Expressao mal formatada", window); *erro=1; return (0); }
            }
            else { aborte("Expressao mal formatada", window); *erro=1; return (0); }
        }
        else if ((t.tipo==FECHA_PARENTESES) || (t.tipo==IGUAL)) {
			if (tam_TADPilha(&operador)>0 && tam_TADPilha(&operando)<2)  { aborte("Expressao mal formatada", window); *erro=1; return (0); }   
            if (tipoExp==EXPRESSAO && t.tipo==FECHA_PARENTESES) { aborte("Expressao mal formatada", window); *erro=1; return (0); }
            if (tipoExp==SUB_EXPRESSAO && t.tipo==IGUAL) { aborte("Expressao mal formatada", window); *erro=1; return (0); }
            if (tam_TADPilha(&operando)==1) {
               pop_TADPilha(&operando, &resultado);
               free_TADPilha(&operando);
               free_TADPilha(&operador);
               return resultado;
            }
            else {
                pop_TADPilha(&operando, &y);
                pop_TADPilha(&operando, &x);
                pop_TADPilha(&operador, &op);
                free_TADPilha(&operando);
                free_TADPilha(&operador);
                if (op==SOMA) return (x+y);
                else if (op==SUBTRAI) return (x-y);
                else if (op==MULTIPLICA) return (x*y);
                else if (op==DIVIDE) return (x/y);
                else { aborte("Expressao mal formatada", window); *erro=1; return (0); }
            }
        }
        else { aborte("Expressao mal formatada", window); *erro=1; return (0); }
    }
}

void toPosfix(TIPO_EXP tipoExp, const char *expr, int *pIdx, int *erro, GtkWidget *window, char exprPf[]) {

    TOKEN t;
    float op;
    BOOL ocorreuOperando=FALSO;
    TAD_PILHA operador;
	char aux[10];
	float vAux;
	char sinal[2];

    init_TADPilha(&operador, 5);

    while (1) {
        t=next_token(expr, pIdx, erro, window); // busca o proximo elemento (token) da entrada
        if (t.tipo==ABRE_PARENTESES || t.tipo==OPERANDO) {
            if (t.tipo==ABRE_PARENTESES) toPosfix(SUB_EXPRESSAO, expr, pIdx, erro, window, exprPf);
            else {
				vAux=t.valOperando;
				if (vAux < 0) { 
					strcpy(sinal,"~"); 
					vAux = -vAux;
				}
				else strcpy(sinal,"");
				sprintf(aux, "%5.2f ", vAux);
				strcat(exprPf, sinal);
				strcat(exprPf, aux);
			}
            ocorreuOperando=VERDADEIRO;
            if (at_top_TADPilha(&operador)==MULTIPLICA || at_top_TADPilha(&operador)==DIVIDE) {
                pop_TADPilha(&operador, &op);
                sprintf(aux, "%c ", (op==MULTIPLICA?'*':'/'));
				strcat(exprPf, aux);
            }
        }
        else if (t.tipo==OPERADOR) {
            if (!ocorreuOperando) { aborte("Expressao mal formatada", window); *erro=1; return; }
            else if ((t.codOperador==SOMA) || (t.codOperador==SUBTRAI)) {
                if (tam_TADPilha(&operador)>0) {
                    pop_TADPilha(&operador, &op);
                    sprintf(aux, "%c ", (op==SOMA?'+':'-'));
					strcat(exprPf, aux);
                }
            }
            push_TADPilha(&operador, t.codOperador);
            ocorreuOperando=FALSO;
        }
        else if ((t.tipo==FECHA_PARENTESES) || (t.tipo==IGUAL)) {
            if ((t.tipo==FECHA_PARENTESES && tipoExp==EXPRESSAO) ||
                (t.tipo==IGUAL && tipoExp==SUB_EXPRESSAO)) {
                aborte("Expressao mal formatada", window); 
				*erro=1; 
				return;
            }
            if (tam_TADPilha(&operador)>0) {
				if (!ocorreuOperando) { aborte("Expressao mal formatada", window); *erro=1; return; }
                pop_TADPilha(&operador, &op);
                sprintf(aux, "%c ", (op==SOMA?'+':(op==SUBTRAI?'-':(op==MULTIPLICA?'*':(op==DIVIDE?'/':'@')))));
				strcat(exprPf, aux);
            }
            free_TADPilha(&operador);
            return;
        }
        else { 
		    aborte("Expressao mal formatada", window); 
			*erro=1; 
			return; 
		}
    }
}


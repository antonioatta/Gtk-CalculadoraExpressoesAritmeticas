#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include <string.h>
#include "TAD_Pilha.h"
#include "Expressao.h"

typedef
struct {
	GtkWidget **widgetsV;
	gchar      *exprM;
} COPIAR_COLAR_REG;


void on_gtk_sair_button_release_event();
void on_gtk_janela_principal_destroy();
void on_gtk_botao_resolver_button_press_event(GtkWidget *, GtkWidget *[]);
void on_gtk_botao_posfixo_button_press_event(GtkWidget *, GtkWidget *[]);
gboolean on_gtk_texto_expr_key_release_event(GtkWidget *, GdkEventKey *, GtkWidget *[]);
void on_gtk_copiar_activate(GtkMenuItem *, gpointer);
void on_gtk_colar_activate(GtkMenuItem *, gpointer);
void on_gtk_sobre_activate(GtkMenuItem *, GtkWindow *);


int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window, *entry, *show_result, *show_pf;
	GtkWidget		*edit_copiar, *edit_colar, *ajuda_sobre;
	GtkWidget 		*btn_resolver, *btn_posfixo;
	GtkWidget	    *widgets[4];
	gchar			exprMem[500] = "";
	COPIAR_COLAR_REG	regCC;

 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Calculadora.ui", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_janela_principal"));

    entry = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_texto_expr")); 
	
	show_pf = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_texto_expr_pf"));
	
	show_result = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_texto_resultado"));
	
    widgets[0]=window; 	widgets[1]=entry;  widgets[2]=show_pf;  widgets[3]=show_result; 
	
	btn_posfixo = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_botao_posfixo")); 
    g_signal_connect(btn_posfixo, "pressed", G_CALLBACK (on_gtk_botao_posfixo_button_press_event), widgets);
	
    btn_resolver = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_botao_resolver")); 
    g_signal_connect(btn_resolver, "pressed", G_CALLBACK (on_gtk_botao_resolver_button_press_event), widgets);
	
	g_signal_connect(entry, "key_release_event", G_CALLBACK (on_gtk_texto_expr_key_release_event), widgets);
	
	regCC.widgetsV=widgets;
	regCC.exprM=exprMem;
	
	edit_copiar = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_copiar"));
	g_signal_connect(edit_copiar, "activate", G_CALLBACK (on_gtk_copiar_activate), &regCC);
	
	edit_colar = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_colar"));
	g_signal_connect(edit_colar, "activate", G_CALLBACK (on_gtk_colar_activate), &regCC);
	
	ajuda_sobre = GTK_WIDGET(gtk_builder_get_object(builder, "gtk_sobre"));
	g_signal_connect(ajuda_sobre, "activate", G_CALLBACK (on_gtk_sobre_activate), GTK_WINDOW(window));
	
    gtk_builder_connect_signals(builder, NULL);
     
    g_object_unref(builder);
 
    gtk_widget_show(window);

    gtk_main();

    return 0;
}
 

// called when "Sair" button is pressed
void on_gtk_sair_button_release_event()
{
    gtk_main_quit();
} 

// called when "Resolver" button is pressed
void on_gtk_botao_resolver_button_press_event(GtkWidget *thisButton, GtkWidget *widgets[])
{
     gchar expr[500], resul[30];
     int idx=0;
     int erro=0;
     float resultado;
	 	 
	 GtkWidget *window, *entry, *show_result;
	 
	 window = widgets[0];
	 entry = widgets[1];
	 show_result = widgets[3];

     strcpy(expr, gtk_entry_get_text(GTK_ENTRY(entry)));
     
	 resultado=calcula(EXPRESSAO, expr, &idx, &erro, window);

     if (!erro) {
     	sprintf(resul, "%10.4f", resultado);
     	gtk_entry_set_text(GTK_ENTRY(show_result), resul);
		gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_result), GTK_ENTRY_ICON_SECONDARY, NULL);
     }
}


// called when "Posfixo" button is pressed
void on_gtk_botao_posfixo_button_press_event(GtkWidget *thisButton, GtkWidget *widgets[])
{
     gchar expr[500], exprPosfixo[500]="";
     int idx=0;
     int erro=0;
	 	 
	 GtkWidget *window, *entry, *show_pf;
	 
	 window = widgets[0]; 
	 entry = widgets[1];
	 show_pf = widgets[2];
     
	 strcpy(expr, gtk_entry_get_text(GTK_ENTRY(entry)));
	 toPosfix(EXPRESSAO, expr, &idx, &erro, window, exprPosfixo);

	 if (!erro) {
		gtk_entry_set_text(GTK_ENTRY(show_pf), exprPosfixo);
		gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_pf), GTK_ENTRY_ICON_SECONDARY, NULL);
     }
}

// called when one key is released in expression entry
gboolean on_gtk_texto_expr_key_release_event(GtkWidget *thisEntry, GdkEventKey *event, GtkWidget *widgets[]) {
	
	GtkWidget  *show_pf, *show_result;
	
	show_pf = widgets[2];
	show_result = widgets[3];
		
	if (gtk_entry_get_text_length(GTK_ENTRY(show_pf)) > 0)  
		gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_pf), GTK_ENTRY_ICON_SECONDARY, "dialog-warning");
	if (gtk_entry_get_text_length(GTK_ENTRY(show_result)) > 0)
		gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_result), GTK_ENTRY_ICON_SECONDARY, "dialog-warning");
	
	return FALSE;
}

// called when "Editar/Copiar" menu option is activated
void on_gtk_copiar_activate(GtkMenuItem *thisMenuItem, gpointer pReg) {
	
	GtkWidget *entry;
	gchar *exprMem;
	COPIAR_COLAR_REG *d;
	
	d=(COPIAR_COLAR_REG *) pReg;
	entry=d->widgetsV[1];
	exprMem=d->exprM;
	
	strcpy(exprMem, gtk_entry_get_text(GTK_ENTRY(entry)));
}

// called when "Editar/Colar" menu option is activated
void on_gtk_colar_activate(GtkMenuItem *thisMenuItem, gpointer pReg) {
	
	GtkWidget *entry;
	GtkWidget  *show_pf, *show_result;
	gchar *exprMem;
	COPIAR_COLAR_REG *d;
	
	d=(COPIAR_COLAR_REG *) pReg;
	entry=d->widgetsV[1];
	show_pf = d->widgetsV[2];
	show_result = d->widgetsV[3];
	exprMem=d->exprM;
	
	if (strlen(exprMem) > 0) {
	   gtk_entry_set_text(GTK_ENTRY(entry), exprMem);
	   if (gtk_entry_get_text_length(GTK_ENTRY(show_pf)) > 0)  
		   gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_pf), GTK_ENTRY_ICON_SECONDARY, "dialog-warning");
	   if (gtk_entry_get_text_length(GTK_ENTRY(show_result)) > 0)
		   gtk_entry_set_icon_from_icon_name(GTK_ENTRY(show_result), GTK_ENTRY_ICON_SECONDARY, "dialog-warning");
	}
}

// called when "Ajuda/Sobre" menu option is activated
void on_gtk_sobre_activate(GtkMenuItem *thisMenuItem, GtkWindow *parentWindow) {
	
	const gchar *auth[] = { "Antonio Atta <antonio.atta@gmail.com>", NULL };
	
	GError *error = NULL;
	GtkIconTheme *icon_theme;
	GdkPixbuf *pixbuf;

	icon_theme = gtk_icon_theme_get_default();
	pixbuf = gtk_icon_theme_load_icon (icon_theme,
                                   "accessories-calculator", // icon name
                                   32, // icon size
                                   0,  // flags
                                   &error);
	
	gtk_show_about_dialog (parentWindow,
							"authors", auth,
							"comments", "Calculadora de Expressões em modo Infixo",
							"copyright", "Copyright 2017 © Todos os direitos reservados",
							"logo", pixbuf,
							"program-name", "Calculadora de Expressões Aritméticas",
							"version", "1.0",
							NULL);
}
							

// called when destroy window (X on top right of the window)
void on_gtk_janela_principal_destroy()
{
    gtk_main_quit();
} 



	
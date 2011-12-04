//
//  tabelas.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/23/11.
//  Copyright 2011 Poli USP. All rights reserved.
//


#include "estruturas.h"

void criaTabelaPalavrasReservadas(noLista **palavraReservada);
void populaTabelaPalavrasReservadas(noLista **palavraReservada);
int buscaSimboloPalavraReservada(char *palavra, noLista **lista);

void criaTabelaSimbolos(noLista **simbolos);
void adicionaSimboloNaTabela(char *palavra,char* valor, noLista **simbolos);
int buscaTabelaSimbolos(char *palavra, noLista **simbolos);


void criaTabelaStrings(noLista **strings);
int adicionaString(char *palavra, noLista **strings);
int buscaTabelaStrings(char *palavra, noLista **strings);

/* FUNCOES CRIADAS PARA TESTE */

void imprimeLista(noLista **lista);
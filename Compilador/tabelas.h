//
//  tabelas.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/23/11.
//  Copyright 2011 Poli USP. All rights reserved.
//


#include "estruturas.h"

void criaTabelaPalavrasReservadas(noLista *palavraReservada);
void populaTabelaPalavrasReservadas(noLista *palavraReservada);

void criaTabelaSimbolos();
void adicionaSimbolo();
void buscaSimbolo();

void criaTabelaStrings();

void criaListaTokens();

/* FUNCOES CRIADAS PARA TESTE */

void imprimeLista(noLista *lista);
//
//  estruturas.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/22/11.
//  Copyright 2011 Poli USP. All rights reserved.
//


#ifndef Compilador_estruturas_h
#define Compilador_estruturas_h

#include "constantes.h"

typedef struct _noLista{
    int identificador; //Identificador
	char *nomeVar;
    char *valorVar; 
    struct _noLista *prox; //Apronta para o próximo da lista
}noLista;

void inicializaLista(noLista **L);
void insereNo(int value, char *palavra, char *valor, noLista **L);
noLista* procuraLista(char *palavra, noLista **L);
int ultimoIdentificador(noLista **L);

typedef struct _token{
    int tipo;
    int linha;
    char *primeiroValor; //valor
    char *segundoValor;
}token;

token* criaToken(int tipo, int linha, char *primeiraPalavra, char *segundaPalavra);
token* procuraToken(int tipo, token *L);

#endif
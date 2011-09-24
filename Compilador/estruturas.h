//
//  estruturas.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/22/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include "constantes.h"




typedef struct _noLista{
    int identificador; //Identificador
    char *valorPalavra; //valor da palvra que será guardada
    struct _noLista *prox; //Apronta para o próximo da lista
}noLista;

void inicializaLista(noLista **L);
void insereNo(int value, char *palavra, noLista **L);
noLista* procuraLista(char *palavra, noLista **L);
int ultimoIdentificador(noLista **L);

typedef struct _token{
    int identificador; //Identificador
    int tipo;
    int linha;
    char *primeiroValor; //valor
    char *segundoValor;
    struct _token *prox;
}token;

void inicializaListaToken(token **T);
void insereToken(int identificador, int tipo, int linha, char *primeiraPalavra, char *segundaPalavra, token **T);
token* procuraToken(int identificador, int tipo, token *L);
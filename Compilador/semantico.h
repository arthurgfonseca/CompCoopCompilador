/*
 *  semantico.h
 *  Compilador
 *
 *  Created by Vininicius Oyama on 12/4/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include "estruturas.h"
#include <stdio.h>

typedef struct _noPilhaString{
	char valor[50];
    struct _noPilhaString *prox; 
} noPilhaString;

void inicializaPilha(noPilhaString** noPilha);
void empilhar(char stringEmpilhada[50], noPilhaString**  noPilha);
void desempilhar(char stringDesempilhada[50], noPilhaString** noPilha);

void inicializarSemantico(FILE* arquivoDeSaida);
void semantico_tbd(token* tokenLido, int acaoSemantica);
int nomeJaExisteNaTabelaDeSimbolos(char* nome);
void gerarCodigoExpressao();
void imprimirVariaveis();
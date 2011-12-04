//
//  estruturas.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/22/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "estruturas.h"
#include <string.h>

//
//
//ESTRUTURA DE LISTA
//
//

void inicializaLista(noLista **L){
    // Inicia lista como NULL
    *L = NULL;
}

//Insere um no na lista
void insereNo(int identificador, char *palavra, char *valor, noLista **L){
    noLista *paux, *pLoop;
    
    if ( *L != NULL) {
        pLoop = *L;
        while( pLoop->prox != NULL){
            pLoop = pLoop->prox; 
        }
    }
    
    
    paux = (noLista *) malloc (sizeof(noLista));
    paux ->identificador = identificador;
    strcpy(paux->nomeVar, palavra);
	strcpy(paux->valorVar, valor);
    paux ->prox = NULL;
    
    
    if ( *L != NULL)
        pLoop->prox = paux;
    else
        *L = paux;
    
}

//Procura na lista usando o identificador e retorna o valor procurado
noLista* procuraLista(char *palavra, noLista **L){
    noLista *paux;
    
    paux = *L;
    
    while(paux != NULL){
        if(strcmp(paux->nomeVar, palavra) == 0) return paux;
        paux = paux->prox;
    } 
    return paux;
}

int ultimoIdentificador(noLista **L) {
    
    noLista *aux, *prox;
    aux = *L;
    if (aux != NULL) {
        prox = aux->prox;
        while (prox != NULL) {
            
            aux = prox;
            prox = aux->prox;
        }
        
        return aux->identificador;
        
    } else {
        return -1;
    }
    
}

//
//
//ESTRUTURA DE TOKENS
//
//

token* criaToken(int tipo, int linha, char *primeiraPalavra, char *segundaPalavra) {
    
    token *taux;
    if (primeiraPalavra == NULL)
		primeiraPalavra = "";
	
	if (segundaPalavra == NULL)
		segundaPalavra = "";
	
    taux = (token *) malloc (sizeof(token));
	int tamanhoDaString = strlen(primeiraPalavra);
    taux ->primeiroValor = (char*) malloc (tamanhoDaString * sizeof(char));
	tamanhoDaString = strlen(segundaPalavra);
    taux ->segundoValor = (char*) malloc (tamanhoDaString * sizeof(char));
	strcpy (taux ->primeiroValor, primeiraPalavra);
	strcpy (taux ->segundoValor, segundaPalavra);
    taux ->linha = linha;
    taux ->tipo = tipo;
    
    return taux;
}


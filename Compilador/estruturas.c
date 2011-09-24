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
void insereNo(int identificador, char *palavra, noLista **L){
    noLista *paux, *pLoop;
    
    if ( *L != NULL) {
        pLoop = *L;
        while( pLoop->prox != NULL){
            pLoop = pLoop->prox; 
        }
    }
    
    paux = (noLista *) malloc (sizeof(noLista));
    paux ->identificador = identificador;
    paux ->valorPalavra = palavra;
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
        if(strcmp(paux->valorPalavra, palavra) == 0) return paux;
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

void inicializaListaToken(token **T) {
    *T = NULL;
}

void insereToken(int identificador, int tipo, int linha, char *primeiraPalavra, char *segundaPalavra, token **T) {
    
    token *taux, *tLoop;
    
    if ( *T != NULL) {
        tLoop = *T;
        while( tLoop->prox != NULL){
            tLoop = tLoop->prox; 
        }
    }
    
    taux = (token *) malloc (sizeof(token));
    taux ->identificador = identificador;
    taux ->primeiroValor = primeiraPalavra;
    taux ->segundoValor = segundaPalavra;
    taux ->linha = linha;
    taux ->prox = NULL;
    if ( *T != NULL)
        tLoop->prox = taux;
    else
        *T = taux;
}

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

token* criaToken(int tipo, int linha, char *primeiraPalavra, char *segundaPalavra) {
    
    token *taux;
    
    taux = (token *) malloc (sizeof(token));
    taux ->primeiroValor = primeiraPalavra;
    taux ->segundoValor = segundaPalavra;
    taux ->linha = linha;
    
    return taux;
}

//int obtemUltimoIdentificadToken(int tipo, token **T) {
//    
//    token *aux, *prox;
//    aux = *T;
//    
//    if (aux != NULL) {
//        int identificador;
//        if (aux->tipo == tipo) {
//            identificador = aux->identificador;
//        }
//        prox = aux->prox;
//        while (prox != NULL) {
//            
//            if (prox->tipo == tipo) {
//                identificador = prox->identificador;
//            }
//            
//            aux = prox;
//            prox = aux->prox;
//        }
//        
//        return identificador;
//        
//    } else {
//        return -1;
//    }
//}

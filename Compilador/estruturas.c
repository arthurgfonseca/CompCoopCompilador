//
//  estruturas.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/22/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include "estruturas.h"

void inicializaLista(noLista *L){
    // Inicia lista como NULL
    L = NULL;
}

//Insere um no na lista
void insereNo(int identificador, char *palavra, noLista *L){
    noLista *novo;
    
    while(L->prox != NULL){
        L = L->prox;
    }    
    novo = (noLista *) malloc(sizeof(noLista));         
    novo->prox = NULL;
    novo->identificador = identificador;
    novo->valorPalavra = palavra;
    L->prox = novo;
    
    
}

//Procura na lista usando o identificador e retorna o valor procurado
noLista procuraLista(int identificador, noLista *L){
    while(L != NULL){
        if(L->identificador == identificador) return *L;
        L = L->prox;
    } 
    return NULL;
}

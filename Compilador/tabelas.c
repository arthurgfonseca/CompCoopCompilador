//
//  tabelas.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/23/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include "tabelas.h"


void criaTabelaPalavrasReservadas(noLista *palavraReservada) {
    
    inicializaLista(palavraReservada);
    
}

void populaTabelaPalavrasReservadas(noLista *palavraReservada) {
    
    
    criaTabelaPalavrasReservadas(palavraReservada);
    
    FILE *entrada;
    char caracterLido;
    int identificador = 0;
    
    entrada = fopen("palavrasreservadas.txt", "r");
    
    while (!feof(entrada)) {
        
        char palavra[MAX];
        fscanf(entrada, "%c", &caracterLido);
        int cont = 0;
        
        
//        Coleta letras do arquivo de entrada até chegar ao final da palavra
        while (caracterLido != '\n') {
            
            palavra[cont] = caracterLido;
            cont ++;
            fscanf(entrada, "%c", &caracterLido);
        }
        
        
        insereNo(identificador, palavra, palavraReservada);
        
        identificador ++;
        
    }
    
    fclose(entrada);
    
}


/* Funcoes criada para teste */

void imprimeLista(noLista *lista) {
    
    noLista *paux; 
    
    paux = lista;
    
    while(paux!=NULL){
        printf(" %d",paux->identificador);
        printf(" %s",paux->valorPalavra);
        printf("\n");
        paux = paux->prox;        
    }    
    printf("\n");
    
}



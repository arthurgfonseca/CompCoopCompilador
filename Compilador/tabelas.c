//
//  tabelas.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/23/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "tabelas.h"

//
//
// Tabela de Palavras reservadas
//
//

void criaTabelaPalavrasReservadas(noLista **palavraReservada) {
    
    inicializaLista(palavraReservada);
    
}

void populaTabelaPalavrasReservadas(noLista **palavraReservada) {
    
    
    FILE *entrada;
    int identificador = 1;
    
    
    entrada = fopen("/Users/arthurgfonseca/Sites/Compilador/Compilador/Compilador/palavrasreservadas.txt", "r");
//    entrada = fopen("palavrasreservadas.txt", "r");
    
    
    while (!feof(entrada)) {
        
     //Verificar ser precisa desalocar da memória
     //TODO: explicitar o tamanho que será alocado
        char *palavraLida = malloc(10);
        
        fscanf(entrada, "%s", palavraLida);
        
        
        insereNo(identificador, palavraLida, palavraReservada);
        
        identificador ++;
        
//        Muda posicao de memoria, para apontar para um lugar vazio
//        palavraLida = palavraLida + 32;
        
        
        
    }
    
    fclose(entrada);
    
    FILE *entradaOperadores;
    
    identificador = 101;
    
    entradaOperadores = fopen("/Users/arthurgfonseca/Sites/Compilador/Compilador/Compilador/operadores.txt", "r");
    
    while (!feof(entradaOperadores)) {
        
        //Verificar ser precisa desalocar da memória
        //TODO: explicitar o tamanho que será alocado
        char *operadorLido = malloc(10);
        
        fscanf(entradaOperadores, "%s", operadorLido);
        
        
        insereNo(identificador, operadorLido, palavraReservada);
        
        identificador ++;
        
//        Muda posicao de memoria, para apontar para um lugar vazio
//        operadorLido = operadorLido + 32;
        
    }
    
}


/* Busca simbolos nas três tabelas possiveis (Palavra reservada, Simbolos, String) */


int buscaSimboloPalavraReservada(char *palavra, noLista **lista) {
    
    noLista *resultado;
    
    resultado = procuraLista(palavra, lista);
    
    if (resultado == NULL) {
        return 0;
    } else {
        printf("achou alguma coisas %d", resultado->identificador);
        return resultado->identificador;
        
    }
    
}

//
//
// TABELA DE SIMBOLOS
//
//

void criaTabelaSimbolos(noLista **simbolos) {
    
    inicializaLista(simbolos);
}

int adicionaSimbolo(char *palavra, noLista **simbolos) {
    
    int identificadorAnterior = ultimoIdentificador(simbolos);
    int identificador;
    if (identificadorAnterior < 0) {
        identificador = 1;
    } else {
        identificador = identificadorAnterior + 1;
    }
    
    
    insereNo(identificador, palavra, simbolos);
    return identificador;
    
}

int buscaTabelaSimbolos(char *palavra, noLista **simbolos) {
    
    noLista *resultado;
    resultado = procuraLista(palavra, simbolos);
    
    if (resultado == NULL) {
        return 0;
    } else {
        
        printf("achou alguma coisas %d", resultado->identificador);
        return resultado->identificador;
    }
}

//
//
// TABELA DE STRINGS
//
//

void criaTabelaStrings(noLista **strings) {
    
    inicializaLista(strings);
    
}

int adicionaString(char *palavra, noLista **strings) {
    
    int identificadorAnterior = ultimoIdentificador(strings);
    int identificador;
    identificador = identificadorAnterior + 1;
    
    insereNo(identificador, palavra, strings);
    return identificador;
    
}

int buscaTabelaStrings(char *palavra, noLista **strings) {
    
    noLista *resultado;
    resultado = procuraLista(palavra, strings);
    
    if (resultado == NULL) {
        return 0;
    } else {
        printf("achou alguma coisas %d", resultado->identificador);
        return resultado->identificador;
    }
    
}

//
//
/* Funcoes criada para teste */
//
//

void imprimeLista(noLista **lista) {
    
    noLista *paux; 
    
    paux = *lista;
    
    while(paux!=NULL){
        printf("\nidentificador %d",paux->identificador);
        printf("\npalavra %s",paux->valorPalavra);
        printf("\n");
        paux = paux->prox;        
    }    
    printf("\n");
    
}

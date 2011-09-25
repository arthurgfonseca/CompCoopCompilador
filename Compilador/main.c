//
//  main.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/21/11.
//  Copyright 2011 Poli USP. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "tabelas.h"
#include "token.h"



// Função criada apenas para imprimir o token, já que a parte sintatica não está implementada
void imprimeToken(token *tokenObtido);


void imprimeToken(token *tokenObtido) {
    
    printf("\n\n---- TOKEN GERADO ---- \n\n");
    printf(" tipo -> %d", tokenObtido->tipo);
    printf("\n linha -> %d", tokenObtido ->linha);
    printf("\n primeiroValor -> %s", tokenObtido->primeiroValor);
    printf("\n segundoValor -> %s", tokenObtido->segundoValor);
    printf("\n\n---- FIM ----");
    
    
    
}

int main (int argc, const char * argv[])
{
    printf("Entrei funcao main");
/*******************************************/    
//    INICIALIZA LISTAS
/*******************************************/    
    
//    Inicializa palavra reservada
    noLista *palavraReservada;
    criaTabelaPalavrasReservadas(&palavraReservada);
    
//    Inicializa simbolos
    noLista *simbolos;
    criaTabelaSimbolos(&simbolos);
    
//    Inicializa strings
    noLista *strings;
    criaTabelaStrings(&strings);
    
//    Token que será obtido
    token *tokemObtido;
    
    
/*******************************************/    
//    POPULA TABELA PALAVRAS RESERVADAS
/*******************************************/
    
    populaTabelaPalavrasReservadas(&palavraReservada);
    
    
/*******************************************/    
//    AREA DE TESTE - GERA TOKEN
/*******************************************/
    
//    char *palavraTeste = "if";
//    char *palavraTeste = "else";
//    char *palavraTeste = "while";
//    char *palavraTeste = "cont";
//    char *palavraTeste = "\"if\"";
    char *palavraTeste = "12";
//    char *palavraTeste = "arthur1";
    char *segundaPalavra = "123";
    
    printf("\n valor PALAVRA = %d", PALAVRA);
    
//    int tipo = PALAVRA;
//    int tipo = INTEIRO;
    int tipo = FLOAT;
    
    int linha = 10;
    
    if (tipo == PALAVRA) {
        
        tokemObtido = obterTokenPalavra(palavraTeste, linha, tipo, &palavraReservada, &simbolos, &strings);
    } else {
        
        tokemObtido = obterTokenNumero(palavraTeste, segundaPalavra, linha, tipo);
    }
    
    
    
    
    
    imprimeToken(tokemObtido);
    
//    printf("\n\n----- LISTA DE SIMBOLOS ----");
//    
//    imprimeLista(&simbolos);
//    
//    printf("\n\n----- LISTA DE STRINGS ----");
//    
//    imprimeLista(&strings);
    
    
    printf("\n\n--- FIM PROGRAMA ---");
    
    
    return 0;
}


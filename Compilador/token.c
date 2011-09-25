//
//  token.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/24/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "tabelas.h"

//    resultadoBusca também será o identificador do token
//    O token sera identificado pela par "identificador", "tipo"
//    A linha é necessaria para o futuro tratamento de erros
token* obterTokenPalavra(char *palavra, int linha, int tipo, noLista **palavrasReservadas, noLista **simbolos, noLista **strings) {
    
    token *token;
    int resultadoBusca;
    int encontrou = 0;
    int isString = 0;
    char primeiroChar = palavra[0];
    char *comparador = "\"";
    
    
    if (strcmp(&primeiroChar, comparador) == 0) {
        isString = 1;
    }
    
    
    if (isString == 0) {
        
        
        //    Verifica se palavra esta na lista de palavras reservadas!
        resultadoBusca = buscaSimboloPalavraReservada(palavra, palavrasReservadas);
        if (resultadoBusca > 0) {
            encontrou = 1;
            token = criaToken(PALAVRARESERVADA, linha, palavra, NULL);
        }
        
        
        //    Se não é Palavra reservada verifica ser é simbolo
        if (encontrou == 0) {
            resultadoBusca = buscaTabelaSimbolos(palavra, simbolos);
            if (resultadoBusca > 0) {
                encontrou = 1;
                token = criaToken(SIMBOLO, linha, palavra, NULL);
            }
            
        }
        
//        Não foi encontrado nem na tabela de palavras reservadas  ou de simbolos
        if (encontrou == 0) {
            int identificador;
//            Adiciona na tabela de simbolos
            identificador = adicionaSimbolo(palavra, simbolos);
//            Cria token
            token = criaToken(SIMBOLO, linha, palavra, NULL);
            
        }
        
    } else {
        
//        Duvida em relação a criação de um novo registro na tabela de String
        
        
//        Remove as aspas da palavra
        long tamanho = strlen(palavra);
        int cont = 1;
        char palavraSemAspas[tamanho - 2];
        
        while (cont < tamanho - 1) {
            palavraSemAspas[cont - 1] = palavra[cont];
            cont ++;
        }
        
//        Modificar o tamanho da memoria alocada
        char *target = malloc(20);
        strcpy(target, palavraSemAspas);
        
        
        int identificador;
        identificador = adicionaString(target, strings);
        token = criaToken(STRING, linha, target, NULL);
        
        
    }
    
    
    return token;
    
}

token* obterTokenNumero(char *primeiroNumero, char *segundoNumero, int linha, int tipo) {
    
    token *token;
//    No caso de identificador = -1, indica que nenhum item desse tipo foi adicionado na lista de tokents;
    
    if (tipo == INTEIRO) {
        
        token = criaToken(INTEIRO, linha, primeiroNumero, NULL);
        
        
    } else {
        
//        Cria um numero do tipo float, o segundoNumero é a parte decimal
        token = criaToken(FLOAT, linha, primeiroNumero, segundoNumero);
    }
    
    return token;
    
}
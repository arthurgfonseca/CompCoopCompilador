//
//  token.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/24/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "tabelas.h"

void criaListaTokens(token **tokens){
    
    inicializaListaToken(tokens);
}

//    resultadoBusca também será o identificador do token
//    O token sera identificado pela par "identificador", "tipo"
//    A linha é necessaria para o futuro tratamento de erros
void montarTokenPalavra(token **token, char *palavra, int linha, int tipo, noLista **palavrasReservadas, noLista **simbolos, noLista **strings) {
    
    int resultadoBusca;
    bool encontrou = false;
    
//    Verifica se palavra esta na lista de palavras reservadas!
    resultadoBusca = buscaSimboloPalavraReservada(palavra, simbolos);
    if (resultadoBusca > 0) {
        encontrou = true;
        insereToken(resultadoBusca, PALAVRARESERVADA, linha, palavra, NULL, token);
    }
    
    
//    Se não é Palavra reservada verifica ser é simbolo
    if (!encontrou ) {
        
        resultadoBusca = buscaTabelaSimbolos(palavra, simbolos);
        if (resultadoBusca > 0) {
            encontrou = true;
            insereToken(resultadoBusca, SIMBOLO, linha, palavra, NULL, token);
        }
        
    }
    
    if (!encontrou) {
        
    }
    
}

void montarTokenNumero(token **token, char *primeiroNumero, char *segundoNumero, int linha, int tipo) {
    
    
}
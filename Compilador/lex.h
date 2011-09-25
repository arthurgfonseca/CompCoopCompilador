/*
 *  lex.h
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "estruturas.h"

void inicializarAnalizadorLexico();
token getToken(FILE* entradaLida);
token obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida);
int deveConcatenarOCaracterLidoAoLexema();
void concatenarCharNaString(char caractereASerConcatenado, char* stringQueVaiReceberOChar);
int encontrouToken();
token gerarTokenDeFimDeArquivo();
token gerarTokenAPartirDoLexemaEncontrado(char* lexemaEncontrado);
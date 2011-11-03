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
token* getToken(FILE* entradaLida);
token* obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida);
int deveConcatenarOCaracterLidoAoLexema();
int deveReiniciarOLexema();
void concatenarCharNaString(char caractereASerConcatenado, char* stringQueVaiReceberOChar);
void incrementarNumeroDaLinhaLidaCasoNecessario(char caractereLido);
int encontrouToken();
token* gerarTokenDeFimDeArquivo();
token* gerarTokenAPartirDoLexemaEncontrado(char* lexemaEncontrado);
int obterIdUnicoDoTokenDePalavraReservada(token* tokenDePalavraReservada);
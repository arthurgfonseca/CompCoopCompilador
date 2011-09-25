/*
 *  lex.h
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

void inicializarAnalizadorLexico();
int getToken(FILE* entradaLida);
int obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida);
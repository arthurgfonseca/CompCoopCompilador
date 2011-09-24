//
//  token.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/24/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include "estruturas.h"

void criaListaTokens(token **tokens);
void montarTokenPalavra(token **token, char *palavra, int linha, int tipo, noLista **palavrasReservadas, noLista **simbolos, noLista **strings);
void montarTokenNumero(token **token, char *primeiroNumero, char *segundoNumero, int linha, int tipo);
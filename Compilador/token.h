//
//  token.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/24/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

#include "estruturas.h"

token* obterTokenPalavra(char *palavra, int linha, int tipo, noLista **palavrasReservadas, noLista **simbolos, noLista **strings);
token* obterTokenNumero(char *primeiroNumero, char *segundoNumero, int linha, int tipo);
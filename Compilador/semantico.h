/*
 *  semantico.h
 *  Compilador
 *
 *  Created by Vininicius Oyama on 12/4/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include "estruturas.h"

int semantico_tbd(noLista** tabelaDeSimbolos, token* tokenLido, int acaoSemantica);
int nomeJaExisteNaTabelaDeSimbolos(char* nome, noLista** tabelaDeSimbolos);
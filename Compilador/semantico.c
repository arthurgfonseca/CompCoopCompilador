/*
 *  semantico.c
 *  Compilador
 *
 *  Created by Vininicius Oyama on 12/4/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include "semantico.h"
#include "constantes.h"
#include "tabelas.h"

int semantico_tbd(noLista **tabelaDeSimbolos, token *tokenLido, int acaoSemantica) {
	int deuErro = FALSE;
	
	if (tokenLido->tipo == SIMBOLO) {
		int a = nomeJaExisteNaTabelaDeSimbolos(tokenLido->primeiroValor, tabelaDeSimbolos);
		adicionaSimboloNaTabela(tokenLido->primeiroValor, "0", tabelaDeSimbolos);
	}
	
	return deuErro;
}	

int nomeJaExisteNaTabelaDeSimbolos(char* nome, noLista** tabelaDeSimbolos) {
	if (buscaTabelaSimbolos(nome, tabelaDeSimbolos) == 0)
		return FALSE;
	
	return TRUE;
}
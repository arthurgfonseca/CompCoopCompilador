/*
 *  semantico.c
 *  Compilador
 *
 *  Created by Vininicius Oyama on 12/4/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "semantico.h"
#include "constantes.h"
#include "tabelas.h"

int semantico_tbd(noLista **tabelaDeSimbolos, token *tokenLido, int acaoSemantica, FILE* saida) {
	int deuErro = FALSE;
	char nomeDaVariavel[50];
	
	switch (acaoSemantica) {
		case ACAOSEMANTICA_INICIO_DO_PROGRAMA:
			fprintf(saida, "JP EXECUTE\n");
			break;
		case ACAOSEMANTICA_DECLARACAO_DE_VARIAVEL:
			
			strcpy(nomeDaVariavel, "");
			strcat(nomeDaVariavel, "Global-");
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (tokenLido->tipo == SIMBOLO) {
				if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel, tabelaDeSimbolos) == TRUE) {
					printf("\nErro. Variavel %s já declarada.", tokenLido->primeiroValor);
					exit(1);
				}
				adicionaSimboloNaTabela(nomeDaVariavel, "0", tabelaDeSimbolos);
			}
			break;
		default:
			break;
	}
	
	return deuErro;
}	

int nomeJaExisteNaTabelaDeSimbolos(char* nome, noLista** tabelaDeSimbolos) {
	if (buscaTabelaSimbolos(nome, tabelaDeSimbolos) == 0)
		return FALSE;
	
	return TRUE;
}
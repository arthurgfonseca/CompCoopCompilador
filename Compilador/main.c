//
//  main.c
//  Compilador
//
//  Created by Arthur Fonseca on 9/21/11.
//  Copyright 2011 Poli USP. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "ape.h"
#include "globais.c"

noLista *tabelaDeSimbolos;

// Função criada apenas para imprimir o token
void imprimeToken(token* tokenObtido);
void semantico_tbd();

void imprimeToken(token* tokenObtido) {
    printf("\n\n---- TOKEN GERADO ---- \n\n");
    printf(" tipo -> %d", tokenObtido->tipo);
    printf("\n primeiroValor -> %s", tokenObtido->primeiroValor);
    printf("\n segundoValor -> %s", tokenObtido->segundoValor);
    printf("\n\n---- FIM ----");
}

void semantico_tbd() {
	printf("TODO \n");
}	

int main (int argc, const char * argv[])
{
	
	numeroDaLinhaLidaNoArquivoFonte = 1;

	FILE* entrada;
	criaTabelaSimbolos(&tabelaDeSimbolos);
	
	entrada=fopen(PATH_PARA_ARQUIVO_FONTE ,"r");
	
	token* tokenLido;
	
	tokenLido = getToken(entrada);
	inicilizarAPE();
	int transicaoEncontrada = TRUE;
	int posicaoTabelaDeSimbolos = 0;
	if (entrada != NULL) 
		while (tokenLido->tipo != EOF && transicaoEncontrada == TRUE) {
			if (tokenLido->tipo == SIMBOLO)
				posicaoTabelaDeSimbolos = adicionaSimbolo(tokenLido, &tabelaDeSimbolos);
			
			if (tokenLido->tipo == PALAVRARESERVADA)
				transicaoEncontrada = transitarAPE(obterIdUnicoDaPalavraReservada(tokenLido));
			else
				transicaoEncontrada = transitarAPE(tokenLido->tipo);
			imprimeToken(tokenLido);
			tokenLido = getToken(entrada);
			
			semantico_tbd();
		}
	
	//imprimeToken(tokenLido);
	

	fclose (entrada);    
	
	if (linguagemAceitaPeloAPE() == TRUE)
		printf("\n \n ACEITOU");
	else 
		printf("\n \n NAO ACEITOU");
	

	
	/*
	int transicaoEncontrada;
	transicaoEncontrada = transitarAPE(1);
	transicaoEncontrada = transitarAPE(1);
	transicaoEncontrada = transitarAPE(1);
	transicaoEncontrada = transitarAPE(1);
	transicaoEncontrada = transitarAPE(2);
	transicaoEncontrada = transitarAPE(2);
	transicaoEncontrada = transitarAPE(2);
	transicaoEncontrada = transitarAPE(2);
	
	printf( "%d", linguagemAceitaPeloAPE());
	 */
    return 0;
}


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

// Função criada apenas para imprimir o token, já que a parte sintatica não está implementada
void imprimeToken(token* tokenObtido);

void imprimeToken(token* tokenObtido) {
    printf("\n\n---- TOKEN GERADO ---- \n\n");
    printf(" tipo -> %d", tokenObtido->tipo);
    printf("\n primeiroValor -> %s", tokenObtido->primeiroValor);
    printf("\n segundoValor -> %s", tokenObtido->segundoValor);
    printf("\n\n---- FIM ----");
}

float plus( float a, float b)
{
	
    return (a + b);
	
}


int main (int argc, const char * argv[])
{
	/*
	numeroDaLinhaLidaNoArquivoFonte = 1;

	FILE* entrada;
	
	entrada=fopen(PATH_PARA_ARQUIVO_FONTE ,"r");
	
	token* tokenLido;
	
	tokenLido = getToken(entrada);
	
	if (entrada != NULL) 
		while (tokenLido->tipo != EOF) {
			imprimeToken(tokenLido);
			tokenLido = getToken(entrada);
		}
	
	imprimeToken(tokenLido);
	
	fclose (entrada);    
	*/
	
	inicilizarAPE();
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
    return 0;
}


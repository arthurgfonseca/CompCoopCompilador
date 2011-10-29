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
	float (*operation)( float, float ) = NULL;
	operation = &plus;

    /* Podemos usar a forma curta tambem, como veremos */
	
    printf( "Plus: 2 + 3 = %g\n", (*operation) ( 2, 3) );
	inicilizarAPE();
	transitarAPE(1);
	transitarAPE(1);
	transitarAPE(2);
	transitarAPE(2);
    return 0;
}


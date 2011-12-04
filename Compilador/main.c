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
#include "semantico.h"
#include "globais.c"

noLista *tabelaDeSimbolos;

// Função criada apenas para imprimir o token
void imprimeToken(token* tokenObtido);
void imprimirVariaveis(FILE* saida);

void imprimeToken(token* tokenObtido) {
    printf("\n\n---- TOKEN GERADO ---- \n\n");
    printf(" tipo -> %d", tokenObtido->tipo);
    printf("\n primeiroValor -> %s", tokenObtido->primeiroValor);
    printf("\n segundoValor -> %s", tokenObtido->segundoValor);
    printf("\n\n---- FIM ----");
}

void imprimirVariaveis(FILE* saida) {
	char stringDeSaida[80];
	
    while(tabelaDeSimbolos != NULL){
		strcpy(stringDeSaida, "");
		strcat (stringDeSaida, tabelaDeSimbolos->nomeVar);
		strcat (stringDeSaida," K /");
		strcat (stringDeSaida, tabelaDeSimbolos->valorVar);
		fprintf(saida, stringDeSaida);
		strcat (stringDeSaida,"\n");
        tabelaDeSimbolos = tabelaDeSimbolos->prox;
    } 
	
}

int main (int argc, const char * argv[])
{
	
	numeroDaLinhaLidaNoArquivoFonte = 1;

	FILE* entrada;
	FILE* saida;
	criaTabelaSimbolos(&tabelaDeSimbolos);
	
	entrada=fopen(PATH_PARA_ARQUIVO_FONTE ,"r");
	saida=fopen(PATH_PARA_ARQUIVO_MVN ,"w");
	
	token* tokenLido;
	token* tokenAntigo;
	
	tokenLido = getToken(entrada);
	inicilizarAPE();
	int transicaoEncontrada = TRUE;
	int deuErroSemantico = FALSE;
	int acaoSemantica = -1;
	char comandoMVNASerImpresso[80];
	
	if (entrada != NULL) 
		while (tokenLido->tipo != EOF && transicaoEncontrada == TRUE && deuErroSemantico == FALSE) {		
			
			deuErroSemantico = semantico_tbd(&tabelaDeSimbolos, tokenAntigo, acaoSemantica, saida);
			
			if (tokenLido->tipo == PALAVRARESERVADA)
				transicaoEncontrada = transitarAPE(obterIdUnicoDaPalavraReservada(tokenLido), &acaoSemantica);
			else
				transicaoEncontrada = transitarAPE(tokenLido->tipo, &acaoSemantica);
			imprimeToken(tokenLido);
			tokenAntigo = tokenLido;
			tokenLido = getToken(entrada);
		}
	
	imprimirVariaveis(saida);
	
	fclose (entrada);    
	
	fclose (saida);    
	
	
	if (linguagemAceitaPeloAPE() == TRUE)
		printf("\n \n ACEITOU LINGUAGEM");
	else 
		printf("\n \n NAO ACEITOU A LINGUAGEM");
	
    return 0;
}


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

int contadorDeTemporarios;
char nomeUltimaVariavelDeAtribuicao[50];
noPilhaString* pilhaDeOperadores;
noPilhaString* pilhaDeOperandos;

void inicializaPilha(noPilhaString** noPilha) {
	*noPilha = NULL;
}

void empilhar(char stringEmpilhada[50], noPilhaString** noPilha) {
	noPilhaString *novoNo;

    novoNo = (noPilhaString *) malloc (sizeof(noPilhaString));
    strcpy(novoNo->valor, stringEmpilhada);
    novoNo->prox = *noPilha;
	*noPilha = novoNo;
}

void desempilhar(char stringDesempilhada[50], noPilhaString** noPilha) {
	if (*noPilha == NULL) {
		strcpy(stringDesempilhada, "");
		return;
	}
	strcpy(stringDesempilhada, (*noPilha)->valor);
	*noPilha = (*noPilha)->prox;
}

void inicializarSemantico() {
	contadorDeTemporarios = 0;
	inicializaPilha(&pilhaDeOperadores);
	inicializaPilha(&pilhaDeOperandos);
	strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
}

int semantico_tbd(noLista **tabelaDeSimbolos, token *tokenLido, int acaoSemantica, FILE* saida) {
	int deuErro = FALSE;
	int devePararDeGerarCodigo;
	char nomeDaVariavel[50];
	
	strcpy(nomeDaVariavel, (char*)(""));
	
	switch (acaoSemantica) {
		case ACAOSEMANTICA_INICIO_DO_PROGRAMA:
			fprintf(saida, "JP EXECUTE\n");
			break;
			
		case ACAOSEMANTICA_INICIO_DO_EXECUTE:
			fprintf(saida, "EXECUTE ");
			break;
			
		case ACAOSEMANTICA_DECLARACAO_DE_VARIAVEL:			
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (tokenLido->tipo == SIMBOLO) {
				if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel, tabelaDeSimbolos) == TRUE) {
					printf("\nErro semantico. Variavel %s já declarada.", tokenLido->primeiroValor);
					return TRUE;
				}
				adicionaSimboloNaTabela(nomeDaVariavel, "0", tabelaDeSimbolos);
			}
			break;	
			
		case ACAOSEMANTICA_ATRIBUICAO_DE_VARIAVEL_INICIO:			
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (tokenLido->tipo == SIMBOLO) {
				if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel, tabelaDeSimbolos) == FALSE) {
					printf("\nErro. Variavel %s não declarada. Linha %d", tokenLido->primeiroValor, tokenLido->linha);
					exit(1);
				}
			}
			
			strcpy(nomeUltimaVariavelDeAtribuicao, nomeDaVariavel);
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_INTEIRO:
			strcat(nomeDaVariavel, "k");
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel, tabelaDeSimbolos) == FALSE) 
				adicionaSimboloNaTabela(nomeDaVariavel, tokenLido->primeiroValor, tabelaDeSimbolos);
			
			empilhar(nomeDaVariavel, &pilhaDeOperandos);
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_SIMBOLO:
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel, tabelaDeSimbolos) == FALSE) {
				printf("\nErro. Variavel %s não declarada. Linha %d", tokenLido->primeiroValor, tokenLido->linha);
				exit(1);
			}
			
			empilhar(nomeDaVariavel, &pilhaDeOperandos);
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_ARITMETICO:
			devePararDeGerarCodigo = FALSE;
					
			while (devePararDeGerarCodigo == FALSE && pilhaDeOperadores != NULL) {
				if ((
					 (strcmp(tokenLido->primeiroValor, (char*)("+")) == 0) ||
					 (strcmp(tokenLido->primeiroValor, (char*)("-")) == 0)
					 )
					&
					(
					 (strcmp(pilhaDeOperadores->valor, (char*)("*")) == 0) ||
					 (strcmp(pilhaDeOperadores->valor, (char*)("/")) == 0) ||
					 (strcmp(pilhaDeOperadores->valor, (char*)("+")) == 0) ||
					 (strcmp(pilhaDeOperadores->valor, (char*)("-")) == 0)
					 ))
						gerarCodigoExpressao(saida, tabelaDeSimbolos);
				else if ((
						  (strcmp(tokenLido->primeiroValor, (char*)("/")) == 0) ||
						  (strcmp(tokenLido->primeiroValor, (char*)("*")) == 0)
						)
						&
						(
						 (strcmp(pilhaDeOperadores->valor, (char*)("*")) == 0) ||
						 (strcmp(pilhaDeOperadores->valor, (char*)("/")) == 0)
						)) 
						gerarCodigoExpressao(saida, tabelaDeSimbolos);
				else 
					devePararDeGerarCodigo = TRUE;		
				
			}
			
			empilhar(tokenLido->primeiroValor, &pilhaDeOperadores);
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES:
			empilhar((char*) "(", &pilhaDeOperadores);
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES:
			if (pilhaDeOperadores != NULL)
				while (strcmp(pilhaDeOperadores->valor, "(") != 0) 
					gerarCodigoExpressao(saida, tabelaDeSimbolos);
			
			desempilhar(nomeDaVariavel, &pilhaDeOperadores);
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_FIM:
			while (pilhaDeOperandos != NULL && pilhaDeOperadores != NULL)
				gerarCodigoExpressao(saida, tabelaDeSimbolos);
			
			break;
			
		case ACAOSEMANTICA_COMANDO_FIM:
			if (strcmp(nomeUltimaVariavelDeAtribuicao, (char*) ("")) != 0)
				fprintf(saida, "MM %s\n", nomeUltimaVariavelDeAtribuicao);
				
			strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
			break;
	}
	
	return deuErro;
}	

int nomeJaExisteNaTabelaDeSimbolos(char* nome, noLista** tabelaDeSimbolos) {
	if (buscaTabelaSimbolos(nome, tabelaDeSimbolos) == 0)
		return FALSE;
	
	return TRUE;
}

void gerarCodigoExpressao(FILE* saida, noLista** tabelaDeSimbolos) {
	char operandoTopo[50];
	char operandoDepois[50];
	char operador[50];
	
	desempilhar(operandoTopo, &pilhaDeOperandos);
	desempilhar(operandoDepois, &pilhaDeOperandos);
	desempilhar(operador, &pilhaDeOperadores);
	
	if ((strcmp(operandoDepois, (char*)("")) != 0))
		fprintf(saida, "LD %s\n", operandoDepois);
	else 
		fprintf(saida, "LD %s\n", operandoTopo);
	

	
	if ((strcmp(operador, (char*)("")) != 0) && (strcmp(operador, (char*)(")")) != 0) && (strcmp(operador, (char*)("(")) != 0))
		fprintf(saida, "%s %s\n", operador, operandoTopo);
	
	contadorDeTemporarios++;
	sprintf(operandoTopo, "TEMP%d", contadorDeTemporarios);
	fprintf(saida, "MM %s\n", operandoTopo);
	empilhar(operandoTopo, &pilhaDeOperandos);	
	
	if (nomeJaExisteNaTabelaDeSimbolos(operandoTopo, tabelaDeSimbolos) == FALSE) 
		adicionaSimboloNaTabela(operandoTopo, "0", tabelaDeSimbolos);
}
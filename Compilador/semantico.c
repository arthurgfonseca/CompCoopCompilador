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
#include "globais.c"

int contadorDeTemporarios;
int acaoSemanticaAnterior;
int contadorDeSubexpressoes;
char nomeUltimaVariavelDeAtribuicao[50];
noPilhaString* pilhaDeOperadores;
noPilhaString* pilhaDeOperandos;
noLista *tabelaDeSimbolos;
FILE* saida;

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

void inicializarSemantico(FILE* arquivoDeSaida) {
	contadorDeTemporarios = 0;
	contadorDeSubexpressoes = 0;
	inicializaPilha(&pilhaDeOperadores);
	inicializaPilha(&pilhaDeOperandos);
	strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
	saida = arquivoDeSaida;
	criaTabelaSimbolos(&tabelaDeSimbolos);
}
void semantico_tbd(token *tokenLido, int acaoSemantica) {

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
			
		case ACAOSEMANTICA_FIM_DO_PROGRAMA:
			imprimirVariaveis();
			break;
			
		case ACAOSEMANTICA_DECLARACAO_DE_VARIAVEL:			
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (tokenLido->tipo == SIMBOLO) {
				if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == TRUE) {
					printf("\nErro semantico. Variavel %s já declarada.", tokenLido->primeiroValor);
					return TRUE;
				}
				adicionaSimboloNaTabela(nomeDaVariavel, "0", &tabelaDeSimbolos);
			}
			break;	
			
		case ACAOSEMANTICA_COMANDO_LE_SIMBOLO:			
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (tokenLido->tipo == SIMBOLO) {
				if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == FALSE) {
					printf("\nErro. Variavel %s não declarada. Linha %d", tokenLido->primeiroValor, tokenLido->linha);
					exit(1);
				}
			}
			
			strcpy(nomeUltimaVariavelDeAtribuicao, nomeDaVariavel);
			break;
			
		case ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM:
			acaoSemanticaAnterior = ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM;			
			break;
			
		case ACAOSEMANTICA_COMANDO_FIM:
			if (acaoSemanticaAnterior == ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM) {
				fprintf(saida, "MM %s\n", nomeUltimaVariavelDeAtribuicao);
				strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
			}
			
			acaoSemanticaAnterior = -1;
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_INTEIRO:
			strcat(nomeDaVariavel, "k");
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == FALSE) 
				adicionaSimboloNaTabela(nomeDaVariavel, tokenLido->primeiroValor, &tabelaDeSimbolos);
			
			empilhar(nomeDaVariavel, &pilhaDeOperandos);
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_SIMBOLO:
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("Global-"));
			strcat(nomeDaVariavel, tokenLido->primeiroValor);
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == FALSE) {
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
						gerarCodigoExpressao();
				else if ((
						  (strcmp(tokenLido->primeiroValor, (char*)("/")) == 0) ||
						  (strcmp(tokenLido->primeiroValor, (char*)("*")) == 0)
						)
						&
						(
						 (strcmp(pilhaDeOperadores->valor, (char*)("*")) == 0) ||
						 (strcmp(pilhaDeOperadores->valor, (char*)("/")) == 0)
						)) 
						gerarCodigoExpressao();
				else 
					devePararDeGerarCodigo = TRUE;		
				
			}
			
			empilhar(tokenLido->primeiroValor, &pilhaDeOperadores);
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES:
			empilhar((char*) "(", &pilhaDeOperadores);
			contadorDeSubexpressoes++;
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES:
			contadorDeSubexpressoes--;
			
			if (pilhaDeOperadores != NULL)
				while (strcmp(pilhaDeOperadores->valor, "(") != 0) 
					gerarCodigoExpressao(saida, tabelaDeSimbolos);
			
			desempilhar(nomeDaVariavel, &pilhaDeOperadores);
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_FIM:
			if (contadorDeSubexpressoes == 0) {
				while (pilhaDeOperandos != NULL && pilhaDeOperadores != NULL)
					gerarCodigoExpressao();
				
				if (pilhaDeOperandos != NULL)
					if (pilhaDeOperandos->prox == NULL) {
						desempilhar(nomeDaVariavel, &pilhaDeOperandos);
						fprintf(saida, "LD %s\n", nomeDaVariavel);
						pilhaDeOperandos = NULL;
					}
			}	
			break;
	}

}	

int nomeJaExisteNaTabelaDeSimbolos(char* nome) {
	if (buscaTabelaSimbolos(nome, &tabelaDeSimbolos) == 0)
		return FALSE;
	
	return TRUE;
}

void gerarCodigoExpressao() {
	char operandoTopo[50];
	char operandoDepois[50];
	char operador[50];
	
	desempilhar(operandoTopo, &pilhaDeOperandos);
	desempilhar(operandoDepois, &pilhaDeOperandos);
	desempilhar(operador, &pilhaDeOperadores);
	
	fprintf(saida, "LD %s\n", operandoDepois);

	if ((strcmp(operador, (char*)("")) != 0) && (strcmp(operador, (char*)(")")) != 0) && (strcmp(operador, (char*)("(")) != 0)) {
		fprintf(saida, "%s %s\n", operador, operandoTopo);
	
		contadorDeTemporarios++;
		sprintf(operandoTopo, "TEMP%d", contadorDeTemporarios);
		fprintf(saida, "MM %s\n", operandoTopo);
		
		empilhar(operandoTopo, &pilhaDeOperandos);	
		
		if (nomeJaExisteNaTabelaDeSimbolos(operandoTopo) == FALSE) 
			adicionaSimboloNaTabela(operandoTopo, "0", &tabelaDeSimbolos);
	}
}

void imprimirVariaveis() {
	char stringDeSaida[80];
	fprintf(saida, (char*)"\n\n\n");
    while(tabelaDeSimbolos != NULL){
		strcpy(stringDeSaida, (char*)"");
		strcat (stringDeSaida, tabelaDeSimbolos->nomeVar);
		strcat (stringDeSaida,(char*)" K /");
		strcat (stringDeSaida, tabelaDeSimbolos->valorVar);
		strcat (stringDeSaida,(char*)"\n");
		fprintf(saida, (char*)stringDeSaida);
        tabelaDeSimbolos = tabelaDeSimbolos->prox;
    } 
	
}
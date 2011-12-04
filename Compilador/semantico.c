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
int contadorDeIf;
int contadorDeWhile;
int contadorDeSubexpressoes;
int pilhaDeComandosAnteriores[1000];
int pilhaDeCondicionais[1000];
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
	contadorDeWhile = 0;
	contadorDeIf = 0;
	inicializaPilha(&pilhaDeOperadores);
	inicializaPilha(&pilhaDeOperandos);
	strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
	saida = arquivoDeSaida;
	criaTabelaSimbolos(&tabelaDeSimbolos);
	pilhaDeComandosAnteriores[0] = 0;
	pilhaDeCondicionais[0] = 0;
}
void semantico_tbd(token *tokenLido, int acaoSemantica) {

	int devePararDeGerarCodigo;
	char nomeDaVariavel[50];
	
	char comando[80];
	
	strcpy(nomeDaVariavel, (char*)(""));
	strcpy(comando, (char*)(""));
	
	switch (acaoSemantica) {
		case ACAOSEMANTICA_INICIO_DO_PROGRAMA:
			fprintf(saida, "JP EXECUTE\n");
			break;
			
		case ACAOSEMANTICA_INICIO_DO_EXECUTE:
			fprintf(saida, "EXECUTE + 0\n");
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
			pilhaDeComandosAnteriores[0]++;
			pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]] = ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM;			
			break;
			
		case ACAOSEMANTICA_COMANDO_FIM:
			if (pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]] == ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM) {
				fprintf(saida, "MM %s\n", nomeUltimaVariavelDeAtribuicao);
				strcpy(nomeUltimaVariavelDeAtribuicao, (char*)(""));
				pilhaDeComandosAnteriores[0]--;
			}
			
						
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
			if (contadorDeSubexpressoes == 0 || 
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0] - 1] == ACAOSEMANTICA_COMANDO_IF_INICIO) {
				while (pilhaDeOperandos != NULL && pilhaDeOperadores != NULL)
					gerarCodigoExpressao();
				
				if (pilhaDeOperandos != NULL)
					if (pilhaDeOperandos->prox == NULL) {
						desempilhar(nomeDaVariavel, &pilhaDeOperandos);
						fprintf(saida, "LD %s\n", nomeDaVariavel);
						pilhaDeOperandos = NULL;
					}
			}	
			
			//caso seja expressão de comparação faz os calculos da comparacao
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("comparacao-parametro-2"));
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == FALSE) {
				adicionaSimboloNaTabela(nomeDaVariavel, (char*)"0", &tabelaDeSimbolos);
			}
			
			//caso em que termina a expressão e era uma comparação
			switch (pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]) {
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_IGUAL:
					fprintf(saida, "MM %s\n", nomeDaVariavel);
					fprintf(saida, "LD comparacao-parametro-1\n");
					fprintf(saida, "- comparacao-parametro-2\n");
					fprintf(saida, "JZ ");
					pilhaDeComandosAnteriores[0]--;
					break;
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MENOR:
					fprintf(saida, "MM %s\n", nomeDaVariavel);
					fprintf(saida, "LD comparacao-parametro-1\n");
					fprintf(saida, "- comparacao-parametro-2\n");
					fprintf(saida, "JN ");
					pilhaDeComandosAnteriores[0]--;
					break;
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MAIOR:
					fprintf(saida, "MM %s\n", nomeDaVariavel);
					fprintf(saida, "LD comparacao-parametro-2\n");
					fprintf(saida, "- comparacao-parametro-1\n");
					fprintf(saida, "JN ");
					pilhaDeComandosAnteriores[0]--;
					break;
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MAIOR_IGUAL:
					pilhaDeComandosAnteriores[0]--;
					break;
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MENOR_IGUAL:
					pilhaDeComandosAnteriores[0]--;
					break;
				case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_DIFERENTE:
					pilhaDeComandosAnteriores[0]--;
					break;
					
			}
			
			
			break;
			
		case ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO:
			//desempilha todas as operações
			if (contadorDeSubexpressoes == 0 || 
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0] - 1] == ACAOSEMANTICA_COMANDO_IF_INICIO) {
				while (pilhaDeOperandos != NULL && pilhaDeOperadores != NULL)
					gerarCodigoExpressao();
				
				if (pilhaDeOperandos != NULL)
					if (pilhaDeOperandos->prox == NULL) {
						desempilhar(nomeDaVariavel, &pilhaDeOperandos);
						fprintf(saida, "LD %s\n", nomeDaVariavel);
						pilhaDeOperandos = NULL;
					}
			}	
			
			strcpy(nomeDaVariavel, (char*)(""));
			strcat(nomeDaVariavel, (char*)("comparacao-parametro-1"));
			
			if (nomeJaExisteNaTabelaDeSimbolos(nomeDaVariavel) == FALSE) {
				adicionaSimboloNaTabela(nomeDaVariavel, (char*)"0", &tabelaDeSimbolos);
			}
			
			fprintf(saida, "MM %s\n", nomeDaVariavel);
			
			pilhaDeComandosAnteriores[0]++;
			
			if(strcmp(tokenLido->primeiroValor, (char*)">") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MAIOR;
			
		    if(strcmp(tokenLido->primeiroValor, (char*)"<") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MENOR;
			
			if(strcmp(tokenLido->primeiroValor, (char*)"==") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_IGUAL;
			
			if(strcmp(tokenLido->primeiroValor, (char*)">=") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MAIOR_IGUAL;
			
		    if(strcmp(tokenLido->primeiroValor, (char*)"<=") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_MENOR_IGUAL;
			
			if(strcmp(tokenLido->primeiroValor, (char*)"!=") == 0)
				pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_COMPARACAO_DIFERENTE;
				 
			break;
			
		case ACAOSEMANTICA_COMANDO_IF_INICIO:
			contadorDeIf++;
			fprintf(saida, "IF%d +0 \n", contadorDeIf);
			pilhaDeCondicionais[0]++;
			pilhaDeCondicionais[pilhaDeCondicionais[0]] = contadorDeIf;
			pilhaDeComandosAnteriores[0]++;
			pilhaDeComandosAnteriores[pilhaDeComandosAnteriores[0]]=ACAOSEMANTICA_COMANDO_IF_INICIO;
			break;
			
		case ACAOSEMANTICA_COMANDO_IF_FIM_SEM_ELSE:
			fprintf(saida, "IF%dELSE + 0\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			fprintf(saida, "IF%dEND + 0 \n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			pilhaDeCondicionais[0]--;
			pilhaDeComandosAnteriores[0]--;
			break;	
			
		case ACAOSEMANTICA_COMANDO_IF_FIM_COM_ELSE:
			fprintf(saida, "IF%dEND + 0\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			pilhaDeCondicionais[0]--;
			pilhaDeComandosAnteriores[0]--;
			break;	
			
		case ACAOSEMANTICA_COMANDO_IF_INICIO_THEN:
			fprintf(saida, "IF%dTHEN\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			fprintf(saida, "JP IF%dELSE\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			fprintf(saida, "IF%dTHEN + 0\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
			break;
			
		case ACAOSEMANTICA_COMANDO_IF_INICIO_ELSE:
			fprintf(saida, "IF%dELSE +0\n", pilhaDeCondicionais[pilhaDeCondicionais[0]]);
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
	
	if (strcmp(operandoDepois, (char*)("")) != 0) 
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
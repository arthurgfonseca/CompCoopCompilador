/*
 *  ape.c
 *  Compilador
 *
 *  Created by Vininicius Oyama on 10/28/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include "constantes.h"
#include "ape.h"
#include <stdio.h>

noSubmaquina* pilhaDeSubmaquinas;
submaquina* submaquinaAtual;
int algumaSubmaquinaTransitou;
int estaNoEstadoFinal;

void colocarSubmaquinaNaPilha(submaquina* SubmaquinaASerEmpilhada) {
	noSubmaquina* novoElementoDaPilha;
	
	novoElementoDaPilha = (noSubmaquina*) malloc (sizeof(noSubmaquina*));
	novoElementoDaPilha->obterSubmaquina = SubmaquinaASerEmpilhada;
	novoElementoDaPilha->proximo = pilhaDeSubmaquinas;
	
	pilhaDeSubmaquinas = novoElementoDaPilha;
}

submaquina* retirarSubmaquinaDaPilha() {
	
	if (pilhaDeSubmaquinaEstaVazia() == TRUE)
		return NULL;
	
	submaquina* SubmaquinaRetorno;
	noSubmaquina* elementoPilha;
	
	SubmaquinaRetorno = (submaquina*)(pilhaDeSubmaquinas->obterSubmaquina);
	elementoPilha = pilhaDeSubmaquinas;
	pilhaDeSubmaquinas = elementoPilha->proximo;
	
	return SubmaquinaRetorno;
}

int pilhaDeSubmaquinaEstaVazia() {
	if (pilhaDeSubmaquinas == NULL)
		return TRUE;
	else 
		return FALSE;
}

void inicilizarAPE() {
	pilhaDeSubmaquinas = NULL;
//	submaquinaAtual = submaquina1n2nCriarSubmaquina();
	submaquinaAtual = submaquinaProgramaCriarSubmaquina();
	algumaSubmaquinaTransitou = FALSE;
	estaNoEstadoFinal = FALSE;
	return;
}

int transitarAPE(int entradaLida) {
	algumaSubmaquinaTransitou = FALSE;
	submaquinaAtual->transitar(entradaLida, &algumaSubmaquinaTransitou, &estaNoEstadoFinal);
	return algumaSubmaquinaTransitou;
}

int linguagemAceitaPeloAPE() {

	if (pilhaDeSubmaquinaEstaVazia() == TRUE &&
		estaNoEstadoFinal == TRUE)
		return TRUE;
	
	return FALSE;
}

/******************************
******** SUBMAQUINAS **********
******************************/
submaquina* criarSubmaquina(void (*funcaoTransitarDaSubmaquina)(int)) {
	submaquina* novaSubmaquina = (submaquina*) malloc (sizeof(submaquina*));
	novaSubmaquina->estadoAtual = 0;
	novaSubmaquina->transitar = funcaoTransitarDaSubmaquina;
	return novaSubmaquina;
}

void chamarSubmaquinaDaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal) {
	*algumaSubmaquinaTransitou = FALSE;
	if (pilhaDeSubmaquinaEstaVazia() == FALSE) {
		submaquinaAtual = retirarSubmaquinaDaPilha();
		submaquinaAtual->transitar(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);
	} 
}

void substituirSubmaquinaAtualColocandoAAntigaNaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal, submaquina* novaSubmaquina) {
	colocarSubmaquinaNaPilha(submaquinaAtual);
	submaquinaAtual =  novaSubmaquina;
	submaquinaAtual->transitar(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);
}

/*** 1^n2^n ***/
submaquina* submaquina1n2nCriarSubmaquina() {
	return criarSubmaquina(&submaquina1n2nTransitar);
}

void submaquina1n2nTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal) {
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0 && entradaLida == 1)
		submaquinaAtual->estadoAtual = 1;
	else if (submaquinaAtual->estadoAtual == 1) {
		submaquinaAtual->estadoAtual = 2;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, submaquina1n2nCriarSubmaquina());
	} 
	else if (submaquinaAtual->estadoAtual == 2 && entradaLida == 2) {
		submaquinaAtual->estadoAtual = 3;
		*estaNoEstadoFinal = TRUE;
	}
	else {
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);
	}
	return;
}

/*** submaquinas da nossa linguagem ***/
//programa
submaquina* submaquinaProgramaCriarSubmaquina() {
	return criarSubmaquina(&submaquinaProgramaTransitar);
}

void submaquinaProgramaTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal) {
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0 && entradaLida == PALAVRARESERVADA_program) {
		submaquinaAtual->estadoAtual = 1;
	} 
	else if (submaquinaAtual->estadoAtual == 1 && entradaLida == PALAVRARESERVADA_declare) {
		submaquinaAtual->estadoAtual = 2;
	} 
	else if (submaquinaAtual->estadoAtual == 1 && entradaLida == PALAVRARESERVADA_execute) {
		submaquinaAtual->estadoAtual = 3;
	} 
	else if (submaquinaAtual->estadoAtual == 2 && (entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
		submaquinaAtual->estadoAtual = 4;
	} 
	else if (submaquinaAtual->estadoAtual == 2 && entradaLida == PALAVRARESERVADA_void) {
		submaquinaAtual->estadoAtual = 5;
	} 
	else if (submaquinaAtual->estadoAtual == 3) {
		if (entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)
			submaquinaAtual->estadoAtual = 6;
		else {
			submaquinaAtual->estadoAtual = 7;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, submaquinaComandosCriarSubmaquina());
		}
	} 
	else if (submaquinaAtual->estadoAtual == 4 && entradaLida == SIMBOLO) {
		submaquinaAtual->estadoAtual = 25;
	} 
	else if (submaquinaAtual->estadoAtual == 5 && entradaLida == SIMBOLO) {
		submaquinaAtual->estadoAtual = 8;
	} 
	else if (submaquinaAtual->estadoAtual == 6 && entradaLida == SIMBOLO) {
		submaquinaAtual->estadoAtual = 13;
	} 
	else if (submaquinaAtual->estadoAtual == 7 && entradaLida == PALAVRARESERVADA_endprogram) {
		submaquinaAtual->estadoAtual = 9;
	} 
	else if (submaquinaAtual->estadoAtual == 8 && entradaLida == '(') {
		submaquinaAtual->estadoAtual = 10;
	} 
	else if (submaquinaAtual->estadoAtual == 10 && (entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
		submaquinaAtual->estadoAtual = 11;
	} 
	else if (submaquinaAtual->estadoAtual == 10 && entradaLida == ')') {
		submaquinaAtual->estadoAtual = 12;
	} 
	else if (submaquinaAtual->estadoAtual == 11 && entradaLida == SIMBOLO) {
		submaquinaAtual->estadoAtual = 31;
	} 
	else if (submaquinaAtual->estadoAtual == 12 && (entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
		submaquinaAtual->estadoAtual = 14;
	} 
	else if (submaquinaAtual->estadoAtual == 13 && entradaLida == ',') {
		submaquinaAtual->estadoAtual = 6;
	} 
	else if (submaquinaAtual->estadoAtual == 13 && entradaLida == '[') {
		submaquinaAtual->estadoAtual = 16;
	} 
	else if (submaquinaAtual->estadoAtual == 13 && entradaLida == ';') {
		submaquinaAtual->estadoAtual = 17;
	} 
	else if (submaquinaAtual->estadoAtual == 14 && entradaLida == SIMBOLO) {
		submaquinaAtual->estadoAtual = 19;
	} 
	else if (submaquinaAtual->estadoAtual == 15 && entradaLida == PALAVRARESERVADA_enddeclare) {
		submaquinaAtual->estadoAtual = 1;
	} 
	else {
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);
	}
	
	return;
}

//comandos
submaquina* submaquinaComandosCriarSubmaquina() {
	return criarSubmaquina(&submaquinaComandosTransitar);
}

void submaquinaComandosTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal) {
	return;
}

//expressões
submaquina* submaquinaExpressoesCriarSubmaquina() {
	return criarSubmaquina(&submaquinaExpressoesTransitar);
}
void submaquinaExpressoesTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal) {
	return;
}
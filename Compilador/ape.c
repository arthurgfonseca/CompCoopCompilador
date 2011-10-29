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
	submaquinaAtual = submaquina1n2nCriarSubmaquina();
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
		colocarSubmaquinaNaPilha(submaquinaAtual);
		submaquinaAtual = submaquina1n2nCriarSubmaquina();
		submaquinaAtual->transitar(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);
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

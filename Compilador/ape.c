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
	return;
}

void transitarAPE(int entradaLida) {
	submaquinaAtual->transitar(entradaLida);
}

int linguagemAceitaPeloAPE() {
	if (pilhaDeSubmaquinaEstaVazia() == TRUE &&
		submaquinaAtual->estaNoEstadoFinal() == TRUE)
		return TRUE;
	
	return FALSE;
}

/******************************
******** SUBMAQUINAS **********
******************************/
submaquina* criarSubmaquina(void (*funcaoTransitarDaSubmaquina)(int), int (*funcaoDeEstadoFinal)()) {
	submaquina* novaSubmaquina = (submaquina*) malloc (sizeof(submaquina*));
	novaSubmaquina->estadoAtual = 0;
	novaSubmaquina->transitar = funcaoTransitarDaSubmaquina;
	novaSubmaquina->estaNoEstadoFinal = funcaoDeEstadoFinal;
	return novaSubmaquina;
}


/*** 1^n2^n ***/
submaquina* submaquina1n2nCriarSubmaquina() {
	return criarSubmaquina(&submaquina1n2nTransitar, &submaquina1n2nEstaNoEstadoFinal);
}

void submaquina1n2nTransitar(int entradaLida) {
	if (submaquinaAtual->estadoAtual == 0 && entradaLida == 1)
		submaquinaAtual->estadoAtual = 1;
	else if (submaquinaAtual->estadoAtual == 1) {
		submaquinaAtual->estadoAtual = 2;
		colocarSubmaquinaNaPilha(submaquinaAtual);
		submaquinaAtual = submaquina1n2nCriarSubmaquina();
		submaquinaAtual->transitar(entradaLida);
	} 
	else if (submaquinaAtual->estadoAtual == 2 && entradaLida == 2) {
		submaquinaAtual->estadoAtual = 3;
	}
	else {
		submaquinaAtual = retirarSubmaquinaDaPilha();
		if (submaquinaAtual != NULL)
			submaquinaAtual->transitar(entradaLida);
	}
	return;
}

int submaquina1n2nEstaNoEstadoFinal() {
	if (submaquinaAtual->estadoAtual == 4)
		return TRUE;
	
	return FALSE;
}
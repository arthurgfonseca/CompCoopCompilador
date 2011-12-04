/*
 *  ape.h
 *  Compilador
 *
 *  Created by Vininicius Oyama on 10/28/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

#include <stdio.h>
#include "estruturas.h"

typedef struct _submaquina {
    int estadoAtual;
	void (*transitar)(int, int*, int*, token*);
} submaquina;

typedef struct _noSubmaquina{
    struct submaquina* obterSubmaquina;
    struct _noSubmaquina* proximo; 
} noSubmaquina;

void colocarSubmaquinaNaPilha(submaquina* novoNo);
submaquina* retirarSubmaquinaDaPilha();

/**** APE ***/

void inicilizarAPE(FILE* saida);
int transitarAPE(int entradaLida, token* tokenLido);
int linguagemAceitaPeloAPE();

/******************************
 ******** SUBMAQUINAS **********
 ******************************/
submaquina* criarSubmaquina(void (*funcaoTransitarDaSubmaquina)(int, int*, int*, token*));
void chamarSubmaquinaDaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido);
void substituirSubmaquinaAtualColocandoAAntigaNaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* acaoSemantica, submaquina* novaSubmaquina);
	
/*** linguagem exemplo: 1^n2^n ****/
submaquina* submaquina1n2nCriarSubmaquina();
void submaquina1n2nTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido);

/*** submaquinas da nossa linguagem ***/
//programa
submaquina* submaquinaProgramaCriarSubmaquina();
void submaquinaProgramaTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido);

//comandos
submaquina* submaquinaComandosCriarSubmaquina();
void submaquinaComandosTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido);

//expressões
submaquina* submaquinaExpressoesCriarSubmaquina();
void submaquinaExpressoesTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido);
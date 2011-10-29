/*
 *  ape.h
 *  Compilador
 *
 *  Created by Vininicius Oyama on 10/28/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

typedef struct _submaquina {
    int estadoAtual;
	void (*transitar)(int, int*, int*);
} submaquina;

typedef struct _noSubmaquina{
    struct submaquina* obterSubmaquina;
    struct _noSubmaquina* proximo; 
} noSubmaquina;

void colocarSubmaquinaNaPilha(submaquina* novoNo);
submaquina* retirarSubmaquinaDaPilha();

/**** APE ***/

void inicilizarAPE();
int transitarAPE(int entradaLida);
int linguagemAceitaPeloAPE();

/******************************
 ******** SUBMAQUINAS **********
 ******************************/
submaquina* criarSubmaquina(void (*funcaoTransitarDaSubmaquina)(int));
void chamarSubmaquinaDaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal);

/*** linguagem exemplo: 1^n2^n ****/
submaquina* submaquina1n2nCriarSubmaquina();
void submaquina1n2nTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal);

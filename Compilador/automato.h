/*
 *  lex.h
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

typedef struct _automato {
    int estadoAtual;
    int estadoAnterior; 
	int** funcaoDeTransicao;
} automato ;

typedef struct _definicaoDeTransicao {
	int estadoAtual;
	int proximoEstado;
	char caractereLido;
} definicaoDeTransicao;

void inicializarAutomato(automato *automatoASerInicializado, int numeroDeEstados, int estadoInicial);
void inicializarFuncaoDeTransicaoDoAutomato(automato *automatoASerInicializado, int numeroDeEstados, int estadoInicial);
void modificarFuncaoDeTransicao(automato *automatoASerInicializado, definicaoDeTransicao novaTransicao);
void modificarFuncaoDeTransicaoLendoTudo(automato *automatoASerInicializado, int estadoAtual, int proximoEstado);
void modificarFuncaoDeTransicaoLendoDigito(automato *automatoASerInicializado, int estadoAtual, int proximoEstado);
void modificarFuncaoDeTransicaoLendoLetra(automato *automatoASerInicializado, int estadoAtual, int proximoEstado);
void atualizarAutomatoParaProximoEstado(automato *automatoASerAtualizado, char caractereLido);
definicaoDeTransicao construirDefinicaoDeTransicao(int estadoAtual, int proximoEstado, char caractereLido);
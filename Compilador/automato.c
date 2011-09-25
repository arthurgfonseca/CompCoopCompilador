/*
 *  lex.c
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "automato.h"
#include "matrizes.h"


void inicializarAutomato(automato *automatoASerInicializado, int numeroDeEstados, int estadoInicial) {
	automatoASerInicializado->estadoAtual = estadoInicial;
	automatoASerInicializado->estadoAnterior = -1;
	inicializarFuncaoDeTransicaoDoAutomato(automatoASerInicializado, numeroDeEstados, estadoInicial);
}

void inicializarFuncaoDeTransicaoDoAutomato(automato *automatoASerInicializado, int numeroDeEstados, int estadoInicial) {
	automatoASerInicializado->funcaoDeTransicao = alocarMatrizDeInteiros(numeroDeEstados, 256);
	int estado;
	int caractere;
	for (estado = 0; estado < numeroDeEstados; estado++) 
		for (caractere = 0; caractere < 256; caractere++) 
			modificarFuncaoDeTransicaoLendoTudo(automatoASerInicializado, estado, estadoInicial);
}

void reiniciarHistoricoDoAutomato(automato *automatoASerReiniciado) {
	automatoASerReiniciado->estadoAtual = 0;
	automatoASerReiniciado->estadoAnterior = 0;
}

void modificarFuncaoDeTransicao(automato *automatoASerInicializado, definicaoDeTransicao novaTransicao) {
	automatoASerInicializado->funcaoDeTransicao[novaTransicao.estadoAtual][(int)novaTransicao.caractereLido] = novaTransicao.proximoEstado;
}

void modificarFuncaoDeTransicaoLendoTudo(automato *automatoASerInicializado, int estadoAtual, int proximoEstado) {
	int caractere;
    for (caractere = 0; caractere < 256; caractere++) 
		automatoASerInicializado->funcaoDeTransicao[estadoAtual][caractere] = proximoEstado;
}

void modificarFuncaoDeTransicaoLendoDigito(automato *automatoASerInicializado, int estadoAtual, int proximoEstado) {
	int digito;
    for (digito = 48; digito <= 57; digito++)
		automatoASerInicializado->funcaoDeTransicao[estadoAtual][digito] = proximoEstado;
}

void modificarFuncaoDeTransicaoLendoLetra(automato *automatoASerInicializado, int estadoAtual, int proximoEstado) {
	int letra;
    for (letra = 65; letra <= 90; letra++)
		automatoASerInicializado->funcaoDeTransicao[estadoAtual][letra] = proximoEstado;
	for (letra = 97; letra <= 122; letra++)
		automatoASerInicializado->funcaoDeTransicao[estadoAtual][letra] = proximoEstado;
}

void atualizarAutomatoParaProximoEstado(automato *automatoASerAtualizado, char caractereLido) {
	automatoASerAtualizado->estadoAnterior = automatoASerAtualizado->estadoAtual;
	automatoASerAtualizado->estadoAtual = automatoASerAtualizado->funcaoDeTransicao[automatoASerAtualizado->estadoAtual][caractereLido];
}

definicaoDeTransicao construirDefinicaoDeTransicao(int estadoAtual, int proximoEstado, char caractereLido) {
	definicaoDeTransicao novaTransicao;
	novaTransicao.estadoAtual = estadoAtual;
	novaTransicao.proximoEstado = proximoEstado;
	novaTransicao.caractereLido = caractereLido;
	return novaTransicao;
}

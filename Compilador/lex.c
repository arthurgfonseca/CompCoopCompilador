/*
 *  lex.c
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include "lex.h"
#include "automato.h"

#define TABULACAO 9
#define LINE_FEED 10
#define CARRIAGE_RETURN 13


automato transdutor;
int analizadorLexicoInicializado = 0;

void inicializarAnalizadorLexico() {
	inicializarAutomato(&transdutor, 12, 0);
	
	definicaoDeTransicao novaTransicao;
	
	//estado 0
	//nao e necessario setar as transições de espaço vazio, tab, enter pois 
	//ao inicializar o automato todas as transições vao para o estado 0
	
	modificarFuncaoDeTransicaoLendoLetra(&transdutor, 0, 1);
	modificarFuncaoDeTransicaoLendoDigito(&transdutor, 0, 2);
	
	novaTransicao = construirDefinicaoDeTransicao(0, 4, '"'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	novaTransicao = construirDefinicaoDeTransicao(0, 6, '/'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	novaTransicao = construirDefinicaoDeTransicao(0, 11, '>'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 11, '<'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 11, '!'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 11, '='); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 1
	modificarFuncaoDeTransicaoLendoLetra(&transdutor, 1, 1);
	modificarFuncaoDeTransicaoLendoDigito(&transdutor, 1, 1);
	
	//estado 2
	modificarFuncaoDeTransicaoLendoDigito(&transdutor, 2, 2);
	novaTransicao = construirDefinicaoDeTransicao(2, 3, '.'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 3
	modificarFuncaoDeTransicaoLendoDigito(&transdutor, 3, 3);
	
	//estado 4
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 4, 4);
	novaTransicao = construirDefinicaoDeTransicao(4, 5, '"'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 5 -> retorna para o inicial sempre
	
	//estado 6
	novaTransicao = construirDefinicaoDeTransicao(6, 7, '/'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	novaTransicao = construirDefinicaoDeTransicao(6, 9, '*'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 7
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 7, 7);
	
	novaTransicao = construirDefinicaoDeTransicao(7, 8, (char)LINE_FEED); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(7, 8, (char)CARRIAGE_RETURN); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 8 - retorna sempre para o inicial
	
	//estado 9
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 9, 9);
	
	novaTransicao = construirDefinicaoDeTransicao(9, 10, '*'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 10
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 10, 9);
	
	novaTransicao = construirDefinicaoDeTransicao(10, 8, '/'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	//estado 11
	
	novaTransicao = construirDefinicaoDeTransicao(11, 12, '='); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	analizadorLexicoInicializado = 1;
}



int getToken(FILE* entradaLida) {
	if (analizadorLexicoInicializado == 0) 
		inicializarAnalizadorLexico();
	
	return obterTokenDepoisDeIicializarAnalizadorLexico(entradaLida);
}

int obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida) {
	
}

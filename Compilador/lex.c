/*
 *  lex.c
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "automato.h"
#include "estruturas.h"
#include "constantes.h"

automato transdutor;
int analizadorLexicoInicializado = FALSE;
int terminouDeAnalizar = FALSE;

void inicializarAnalizadorLexico() {
	inicializarAutomato(&transdutor, 14, 0);
	
	definicaoDeTransicao novaTransicao;
	
	//estado 0
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 0, 13);
	
	novaTransicao = construirDefinicaoDeTransicao(0, 0, ' '); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)TABULACAO); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)CARRIAGE_RETURN); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)LINE_FEED); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
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
	
	analizadorLexicoInicializado = TRUE;
}



token getToken(FILE* entradaLida) {
	token tokenASerRetornado;
	
	if (analizadorLexicoInicializado == 0) 
		inicializarAnalizadorLexico();
	
	//TODO: O correto é obtero token aqui. gerenciar as tabelas vendo se ele ja existe e inserir nas tabelas
	tokenASerRetornado = obterTokenDepoisDeIicializarAnalizadorLexico(entradaLida);
	
	reiniciarHistoricoDoAutomato(&transdutor); //importante ficar sempre imediatamente antes do return
	
	return tokenASerRetornado;
}

token obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida) {

	char lexemaEncontrado[200];
	//necessário resetar a posicao da memoria que possui o lexema
	lexemaEncontrado[0] = '\0';
	char caractereLido;
	
	if (terminouDeAnalizar == TRUE)
		return gerarTokenDeFimDeArquivo();
		
	while (encontrouToken() == FALSE) {
		if (deveConcatenarOCaracterLidoAoLexema() == TRUE) 
			concatenarCharNaString(caractereLido, &lexemaEncontrado);

		caractereLido = getc(entradaLida);
		
		if (caractereLido == EOF) {
			terminouDeAnalizar = TRUE;
			return gerarTokenAPartirDoLexemaEncontrado(&lexemaEncontrado);;
		}
		
		atualizarAutomatoParaProximoEstado(&transdutor, caractereLido);
	}
	
	fseek(entradaLida, -1, SEEK_CUR);
	
	return gerarTokenAPartirDoLexemaEncontrado(&lexemaEncontrado);;
}

int deveConcatenarOCaracterLidoAoLexema() {
	if (transdutor.estadoAtual == 0 &&			
		(transdutor.estadoAnterior == 0 || 
		transdutor.estadoAnterior == -1 || 
		transdutor.estadoAnterior == 5 ||
		transdutor.estadoAnterior == 8 ||
		transdutor.estadoAnterior == 13)) 
			return FALSE;

	return TRUE;

}

void concatenarCharNaString(char caractereASerConcatenado, char* stringQueVaiReceberOChar) {
	int tamanhoDaString = strlen(stringQueVaiReceberOChar);
	stringQueVaiReceberOChar[tamanhoDaString] = caractereASerConcatenado;
	stringQueVaiReceberOChar[tamanhoDaString + 1] = '\0';
}

int encontrouToken() {
	return (transdutor.estadoAtual == 0 && transdutor.estadoAnterior != 0 && transdutor.estadoAnterior != 8 && transdutor.estadoAnterior != -1);
}

token gerarTokenDeFimDeArquivo() {
	token tokenASerRetornado;
	tokenASerRetornado.tipo = 1;
	printf("TOKEN: EOF \n");
	return tokenASerRetornado;
}

token gerarTokenAPartirDoLexemaEncontrado(char* lexemaEncontrado) {
	token tokenASerRetornado;
	int tipoDoToken;
	
	printf("TOKEN: %s		TIPO:", lexemaEncontrado);
	
	switch (transdutor.estadoAnterior) {
		case 1:
			printf("palavra ");
			tipoDoToken = PALAVRA;
			break;
		case 2:
			printf("int ");
			tipoDoToken = INTEIRO;
			break;
		case 3:
			printf("float ");
			tipoDoToken = FLOAT;
			break;
		case 5:
			printf("string");
			tipoDoToken = STRING;
			break;
		case 11:
			printf("operador");
			tipoDoToken = OPERADOR;
			break;
		case 12:
			printf("operador");
			tipoDoToken = OPERADOR;
			break;
	}
	
	printf("\n");
	
	
	return tokenASerRetornado;
}


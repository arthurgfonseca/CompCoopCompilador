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
#include "globais.c"
#include "tabelas.h"
#include "token.h"

automato transdutor;
int analizadorLexicoInicializado = FALSE;
int terminouDeAnalizar = FALSE;
int deveIncrementarLinhaLida = FALSE;
noLista *palavraReservada;
noLista *simbolos;
noLista *strings;

void inicializarAnalizadorLexico() {
	criaTabelaPalavrasReservadas(&palavraReservada);
    criaTabelaSimbolos(&simbolos);
    criaTabelaStrings(&strings);
    populaTabelaPalavrasReservadas(&palavraReservada);
	
	inicializarAutomato(&transdutor, 15, 0);
	
	definicaoDeTransicao novaTransicao;
	
	//estado 0
	modificarFuncaoDeTransicaoLendoTudo(&transdutor, 0, 13);
	
	novaTransicao = construirDefinicaoDeTransicao(0, 0, WHITE_SPACE); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)TABULACAO); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)CARRIAGE_RETURN); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)LINE_FEED); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	novaTransicao = construirDefinicaoDeTransicao(0, 0, (char)ESPACONULO); 
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
	novaTransicao = construirDefinicaoDeTransicao(1, 1, '_'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
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
	
	//estado 13(qualquer caractere) para o caso de dois pipes 
	novaTransicao = construirDefinicaoDeTransicao(13, 14, '|'); 
	modificarFuncaoDeTransicao(&transdutor, novaTransicao);
	
	analizadorLexicoInicializado = TRUE;
}



token* getToken(FILE* entradaLida) {
	token* tokenASerRetornado;
	
	if (analizadorLexicoInicializado == 0) 
		inicializarAnalizadorLexico();
	
	//TODO: O correto é obtero token aqui. gerenciar as tabelas vendo se ele ja existe e inserir nas tabelas
	tokenASerRetornado = obterTokenDepoisDeIicializarAnalizadorLexico(entradaLida);
	
	reiniciarHistoricoDoAutomato(&transdutor); //importante ficar sempre imediatamente antes do return
	
	return tokenASerRetornado;
}

token* obterTokenDepoisDeIicializarAnalizadorLexico(FILE* entradaLida) {

	char lexemaEncontrado[200];
	//necessário resetar a posicao da memoria que possui o lexema
	lexemaEncontrado[0] = '\0';
	char caractereLido;
	
	if (terminouDeAnalizar == TRUE)
		return gerarTokenDeFimDeArquivo();
	
	while (encontrouToken() == FALSE) {
		if (deveConcatenarOCaracterLidoAoLexema() == TRUE) 
			concatenarCharNaString(caractereLido, lexemaEncontrado);

		if (deveReiniciarOLexema())
			lexemaEncontrado[0] = '\0';
		
		caractereLido = getc(entradaLida);
		incrementarNumeroDaLinhaLidaCasoNecessario(caractereLido);
		
		if (caractereLido == EOF) {
			terminouDeAnalizar = TRUE;
			if (lexemaEncontrado[0] == '\0')
				return gerarTokenDeFimDeArquivo();
			else
				return gerarTokenAPartirDoLexemaEncontrado(lexemaEncontrado);;
		}
		
		atualizarAutomatoParaProximoEstado(&transdutor, caractereLido);
	}
	
	fseek(entradaLida, -1, SEEK_CUR);
	
	return gerarTokenAPartirDoLexemaEncontrado(lexemaEncontrado);;
}

int deveConcatenarOCaracterLidoAoLexema() {
	
	if (transdutor.estadoAtual == 0 &&			
		(transdutor.estadoAnterior == 0 || 
		transdutor.estadoAnterior == -1 || 
		transdutor.estadoAnterior == 8 )) 
			return FALSE;

	if (transdutor.estadoAtual == 7 ||
		transdutor.estadoAtual == 8 ||
		transdutor.estadoAtual == 9 ||
		transdutor.estadoAtual == 10 )
		
		return FALSE;
	
	return TRUE;

}

int deveReiniciarOLexema() {
	
	if (transdutor.estadoAtual == 0 &&			
		(transdutor.estadoAnterior == 7 || 
		 transdutor.estadoAnterior == 8)) 
		return TRUE;
	
	return FALSE;
	
}
			
void concatenarCharNaString(char caractereASerConcatenado, char* stringQueVaiReceberOChar) {
	int tamanhoDaString = strlen(stringQueVaiReceberOChar);
	stringQueVaiReceberOChar[tamanhoDaString] = caractereASerConcatenado;
	stringQueVaiReceberOChar[tamanhoDaString + 1] = '\0';
}

int encontrouToken() {
	return (transdutor.estadoAtual == 0 && transdutor.estadoAnterior != 0 && transdutor.estadoAnterior != 8 && transdutor.estadoAnterior != -1);
}

void incrementarNumeroDaLinhaLidaCasoNecessario(char caractereLido) {
	//no estado de comentario o caractere nao é lido duas vezes. então ja incrementamos e saimos da função
	if (transdutor.estadoAtual == 7 || 
		transdutor.estadoAtual == 8 ||
		transdutor.estadoAtual == 9 || 
		transdutor.estadoAtual == 10) 
		if ((caractereLido == CARRIAGE_RETURN ||
			 caractereLido == LINE_FEED)) 
			numeroDaLinhaLidaNoArquivoFonte++;
			
	
	if (transdutor.estadoAtual == 0 &&  
	    transdutor.estadoAnterior == 0) {
		if ((caractereLido == CARRIAGE_RETURN ||
			 caractereLido == LINE_FEED)) 
			numeroDaLinhaLidaNoArquivoFonte++;
	}
	return;

	
	//deveIncrementarLinhaLida serve para evitar que o numero seja incrementado duas vezes
	//pois todo char é lido duas vezes pelo algoritmo
	if ((caractereLido == CARRIAGE_RETURN ||
		caractereLido == LINE_FEED)) 
		if (deveIncrementarLinhaLida == TRUE) {
			numeroDaLinhaLidaNoArquivoFonte++;
			deveIncrementarLinhaLida = FALSE;
		} else {
			deveIncrementarLinhaLida = TRUE;
		}	
}

token* gerarTokenDeFimDeArquivo() {
	token* tokenASerRetornado;
	tokenASerRetornado = criaToken(EOF, numeroDaLinhaLidaNoArquivoFonte, NULL, NULL);

	return tokenASerRetornado;
}

token* gerarTokenAPartirDoLexemaEncontrado(char* lexemaEncontrado) {
	token* tokenASerRetornado;

	switch (transdutor.estadoAnterior) {
		case 1:
			tokenASerRetornado = obterTokenPalavra(lexemaEncontrado, numeroDaLinhaLidaNoArquivoFonte, PALAVRA, &palavraReservada, &simbolos, &strings);
			break;
		case 2:
			tokenASerRetornado = obterTokenNumero(lexemaEncontrado, "", numeroDaLinhaLidaNoArquivoFonte, INTEIRO);
			break;
		case 3:
			tokenASerRetornado = obterTokenNumero(lexemaEncontrado, "", numeroDaLinhaLidaNoArquivoFonte, FLOAT);
			break;
		case 5:
			tokenASerRetornado = obterTokenPalavra(lexemaEncontrado, numeroDaLinhaLidaNoArquivoFonte, STRING, &palavraReservada, &simbolos, &strings);
			break;
		case 11:
			if (strcmp(lexemaEncontrado,(char*) "=") == 0) 
				tokenASerRetornado = tokenASerRetornado = criaToken('=', numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
			else if (strcmp(lexemaEncontrado,(char*) "!") == 0) 
				tokenASerRetornado = tokenASerRetornado = criaToken('!', numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
			else
				tokenASerRetornado = tokenASerRetornado = criaToken(OPERADOR, numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
				break;
		case 12:
			tokenASerRetornado = tokenASerRetornado = criaToken(OPERADOR, numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
			break;
		case 14:
			tokenASerRetornado = criaToken(OPERADOR_OU, numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
			break;
		default:
			tokenASerRetornado = criaToken((int)lexemaEncontrado[0], numeroDaLinhaLidaNoArquivoFonte, lexemaEncontrado, NULL);
			break;
	}
		
	return tokenASerRetornado;
}

int obterIdUnicoDaPalavraReservada(token* tokenDePalavraReservada) {
	char* palavraReservadaDoToken = tokenDePalavraReservada->primeiroValor;
	
	if (strcmp(palavraReservadaDoToken, (char*) "program") == 0)
		return PALAVRARESERVADA_program;
	if (strcmp(palavraReservadaDoToken, (char*) "execute") == 0)
		return PALAVRARESERVADA_execute;
	if (strcmp(palavraReservadaDoToken, (char*) "endprogram") == 0)
		return PALAVRARESERVADA_endprogram;
	
	if (strcmp(palavraReservadaDoToken, (char*) "declare") == 0)
		return PALAVRARESERVADA_declare;
	if (strcmp(palavraReservadaDoToken, (char*) "return") == 0)
		return PALAVRARESERVADA_return;
	if (strcmp(palavraReservadaDoToken, (char*) "enddeclare") == 0)
		return PALAVRARESERVADA_enddeclare;
	
	if (strcmp(palavraReservadaDoToken, (char*) "while") == 0)
		return PALAVRARESERVADA_while;
	if (strcmp(palavraReservadaDoToken, (char*) "endwhile") == 0)
		return PALAVRARESERVADA_endwhile;
	
	if (strcmp(palavraReservadaDoToken, (char*) "if") == 0)
		return PALAVRARESERVADA_if;
	if (strcmp(palavraReservadaDoToken, (char*) "else") == 0)
		return PALAVRARESERVADA_else;
	if (strcmp(palavraReservadaDoToken, (char*) "endif") == 0)
		return PALAVRARESERVADA_endif;
	
	
	if (strcmp(palavraReservadaDoToken, (char*) "print") == 0)
		return PALAVRARESERVADA_print;
	if (strcmp(palavraReservadaDoToken, (char*) "scan") == 0)
		return PALAVRARESERVADA_scan;
	
	if (strcmp(palavraReservadaDoToken, (char*) "int") == 0)
		return PALAVRARESERVADA_int;
	if (strcmp(palavraReservadaDoToken, (char*) "float") == 0)
		return PALAVRARESERVADA_float;
	if (strcmp(palavraReservadaDoToken, (char*) "void") == 0)
		return PALAVRARESERVADA_void;
}

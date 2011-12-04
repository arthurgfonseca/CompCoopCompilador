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
#include "semantico.h"

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

void inicilizarAPE(FILE* saida) {
	pilhaDeSubmaquinas = NULL;
	submaquinaAtual = submaquinaProgramaCriarSubmaquina();
	algumaSubmaquinaTransitou = FALSE;
	estaNoEstadoFinal = FALSE;
	inicializarSemantico(saida);
	return;
}

int transitarAPE(int entradaLida, token* tokenLido) {
	algumaSubmaquinaTransitou = FALSE;
	submaquinaAtual->transitar(entradaLida, &algumaSubmaquinaTransitou, &estaNoEstadoFinal, tokenLido);
	return algumaSubmaquinaTransitou;
}

int linguagemAceitaPeloAPE() {

	if (pilhaDeSubmaquinaEstaVazia() == TRUE &
		estaNoEstadoFinal == TRUE)
		return TRUE;
	
	return FALSE;
}

/******************************
******** SUBMAQUINAS **********
******************************/
submaquina* criarSubmaquina(void (*funcaoTransitarDaSubmaquina)(int, int*, int*, token*)) {
	submaquina* novaSubmaquina = (submaquina*) malloc (sizeof(submaquina*));
	novaSubmaquina->estadoAtual = 0;
	novaSubmaquina->transitar = funcaoTransitarDaSubmaquina;
	return novaSubmaquina;
}

void chamarSubmaquinaDaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido) {
	*algumaSubmaquinaTransitou = FALSE;
	if (pilhaDeSubmaquinaEstaVazia() == FALSE) {
		submaquinaAtual = retirarSubmaquinaDaPilha();
		submaquinaAtual->transitar(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
	} 
}

void substituirSubmaquinaAtualColocandoAAntigaNaPilha(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal, token* tokenLido , submaquina* novaSubmaquina) {
	colocarSubmaquinaNaPilha(submaquinaAtual);
	submaquinaAtual =  novaSubmaquina;
	submaquinaAtual->transitar(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
}

/*** 1^n2^n ***/
submaquina* submaquina1n2nCriarSubmaquina() {
	return criarSubmaquina(&submaquina1n2nTransitar);
}

void submaquina1n2nTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal, token* tokenLido) {
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0 & entradaLida == 1)
		submaquinaAtual->estadoAtual = 1;
	else if (submaquinaAtual->estadoAtual == 1) {
		submaquinaAtual->estadoAtual = 2;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquina1n2nCriarSubmaquina());
		
	} 
	else if (submaquinaAtual->estadoAtual == 2 & entradaLida == 2) {
		submaquinaAtual->estadoAtual = 3;
		*estaNoEstadoFinal = TRUE;
	}
	else {
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
	}
	return;
}

/*** submaquinas da nossa linguagem ***/
//programa
submaquina* submaquinaProgramaCriarSubmaquina() {
	return criarSubmaquina(&submaquinaProgramaTransitar);
}

void submaquinaProgramaTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido) {
	int naoEncontrouTransicao;
	naoEncontrouTransicao = TRUE;
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0) {
		if (entradaLida == PALAVRARESERVADA_program) {
			submaquinaAtual->estadoAtual = 1;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido,ACAOSEMANTICA_INICIO_DO_PROGRAMA);
		}
	}
	else if (submaquinaAtual->estadoAtual == 1) {
		if (entradaLida == PALAVRARESERVADA_declare) {
			submaquinaAtual->estadoAtual = 2;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_execute) {
			submaquinaAtual->estadoAtual = 3;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_INICIO_DO_EXECUTE);
		}
	}
	else if (submaquinaAtual->estadoAtual == 2) {
		if ((entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_void) {
			submaquinaAtual->estadoAtual = 5;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 3) {
		if ((entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
		else {
			submaquinaAtual->estadoAtual = 7;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaComandosCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 4) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 23;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 8;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 5) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 8;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 6) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_DECLARACAO_DE_VARIAVEL);
		}
	}
	else if (submaquinaAtual->estadoAtual == 7) {
		if (entradaLida == PALAVRARESERVADA_endprogram) {
			submaquinaAtual->estadoAtual = 9;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_FIM_DO_PROGRAMA);
		}
	}
	else if (submaquinaAtual->estadoAtual == 8) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 10;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 10) {
		if ((entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 12;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 11) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 25;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 12) {
		if ((entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
			submaquinaAtual->estadoAtual = 14;
			naoEncontrouTransicao = FALSE;
		}
		else {
			submaquinaAtual->estadoAtual = 15;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaComandosCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 13) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 16;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 3;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 14) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 17;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 15) {
		if (entradaLida == PALAVRARESERVADA_enddeclare) {
			submaquinaAtual->estadoAtual = 1;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 16) {
		if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 19;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 17) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 18;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 14;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 12;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 18) {
		if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 21;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 19) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 20;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 20) {
		if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 3;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 21) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 22;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 22) {
		if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 14;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 12;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 23) {
		if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 24;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 24) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 5;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 25) {
		if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 26;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 12;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 26) {
		if ((entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)) {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
	}
	
	
	if(naoEncontrouTransicao == TRUE) {
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
	}
	return;	
}


//comandos
submaquina* submaquinaComandosCriarSubmaquina() {
	return criarSubmaquina(&submaquinaComandosTransitar);
}

void submaquinaComandosTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido) {
	int naoEncontrouTransicao;
	naoEncontrouTransicao = TRUE;
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 1;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_COMANDO_LE_SIMBOLO);
		}
		else if (entradaLida == PALAVRARESERVADA_scan) {
			submaquinaAtual->estadoAtual = 2;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_print) {
			submaquinaAtual->estadoAtual = 3;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_return) {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_if) {
			submaquinaAtual->estadoAtual = 5;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_while) {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 1) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 7;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 8;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '=') {
			submaquinaAtual->estadoAtual = 9;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 2) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 26;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 3) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 22;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 4) {
		if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_COMANDO_FIM);
		}
		else {
			submaquinaAtual->estadoAtual = 11;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 5) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 18;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 6) {
		if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 10;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 7) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
		else {
			submaquinaAtual->estadoAtual = 19;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 8) {
		submaquinaAtual->estadoAtual = 16;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 9) {
		submaquinaAtual->estadoAtual = 11;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
		semantico_tbd(tokenLido, ACAOSEMANTICA_COMANDO_ATRIBUICAO_DE_VARIAVEL_FIM);
	}
	else if (submaquinaAtual->estadoAtual == 10) {
		submaquinaAtual->estadoAtual = 12;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		*estaNoEstadoFinal = TRUE;
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 11) {
		if (entradaLida == ';') {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_COMANDO_FIM);
		}
	}
	else if (submaquinaAtual->estadoAtual == 12) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 14;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 13) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 1;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_COMANDO_LE_SIMBOLO);
		}
		else if (entradaLida == PALAVRARESERVADA_scan) {
			submaquinaAtual->estadoAtual = 2;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_print) {
			submaquinaAtual->estadoAtual = 3;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_return) {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_if) {
			submaquinaAtual->estadoAtual = 5;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_while) {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 14) {
		submaquinaAtual->estadoAtual = 15;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaComandosCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 15) {
		if (entradaLida == PALAVRARESERVADA_endwhile) {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 16) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 17;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 17) {
		if (entradaLida == '=') {
			submaquinaAtual->estadoAtual = 9;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 18) {
		submaquinaAtual->estadoAtual = 20;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 19) {
		if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 22;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 20) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 21;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 21) {
		submaquinaAtual->estadoAtual = 23;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaComandosCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 22) {
		submaquinaAtual->estadoAtual = 19;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 23) {
		if (entradaLida == PALAVRARESERVADA_endif) {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == PALAVRARESERVADA_else) {
			submaquinaAtual->estadoAtual = 24;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 24) {
		submaquinaAtual->estadoAtual = 25;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaComandosCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 25) {
		if (entradaLida == PALAVRARESERVADA_endif) {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 26) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 27;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 27) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 28;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 28) {
		submaquinaAtual->estadoAtual = 29;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 29) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 30;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 30) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 11;
			naoEncontrouTransicao = FALSE;
		}
	}
	
	
	if(naoEncontrouTransicao == TRUE) {
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
	}
	return;	
}

//expressões
submaquina* submaquinaExpressoesCriarSubmaquina() {
	return criarSubmaquina(&submaquinaExpressoesTransitar);
}
void submaquinaExpressoesTransitar(int entradaLida, int* algumaSubmaquinaTransitou,int* estaNoEstadoFinal,token* tokenLido) {
	int naoEncontrouTransicao;
	naoEncontrouTransicao = TRUE;
	*algumaSubmaquinaTransitou = TRUE;
	*estaNoEstadoFinal = FALSE;
	
	if (submaquinaAtual->estadoAtual == 0) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 1;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_SIMBOLO);
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 2;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES);
		}
		else if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_INTEIRO);
		}
		else if (entradaLida == PALAVRARESERVADA_float) {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '!') {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 1) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 5;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 6;
			naoEncontrouTransicao = FALSE;
		}
		else if ((entradaLida == '+' || entradaLida == '-' || entradaLida == '*' || entradaLida == '/')) {
			submaquinaAtual->estadoAtual = 7;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_ARITMETICO);
		}
		else if (entradaLida == OPERADOR) {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 2) {
		submaquinaAtual->estadoAtual = 17;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 3) {
		if ((entradaLida == '+' || entradaLida == '-' || entradaLida == '*' || entradaLida == '/')) {
			submaquinaAtual->estadoAtual = 7;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_ARITMETICO);
		}
		else if (entradaLida == OPERADOR) {
			submaquinaAtual->estadoAtual = 4;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 4) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 8;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_SIMBOLO);
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 9;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES);
		}
		else if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_INTEIRO);
		}
		else if (entradaLida == PALAVRARESERVADA_float) {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 5) {
		submaquinaAtual->estadoAtual = 18;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 6) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
		else {
			submaquinaAtual->estadoAtual = 19;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 7) {
		if (entradaLida == SIMBOLO) {
			submaquinaAtual->estadoAtual = 21;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_SIMBOLO);
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 22;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES);
		}
		else if (entradaLida == INTEIRO) {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_INTEIRO);
		}
		else if (entradaLida == PALAVRARESERVADA_float) {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 8) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 12;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 13;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_ABRE_PARENTESES);
		}
		else if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 9) {
		submaquinaAtual->estadoAtual = 11;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 10) {
		if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 11) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
	}
	else if (submaquinaAtual->estadoAtual == 12) {
		submaquinaAtual->estadoAtual = 16;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 13) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
		else {
			submaquinaAtual->estadoAtual = 14;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 14) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
		else if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 15;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 15) {
		submaquinaAtual->estadoAtual = 14;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 16) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 10;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 17) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
	}
	else if (submaquinaAtual->estadoAtual == 18) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 19) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 3;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
		else if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 20;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 20) {
		submaquinaAtual->estadoAtual = 19;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 21) {
		if (entradaLida == '[') {
			submaquinaAtual->estadoAtual = 25;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '(') {
			submaquinaAtual->estadoAtual = 26;
			naoEncontrouTransicao = FALSE;
		}
		else if ((entradaLida == '+' || entradaLida == '-' || entradaLida == '*' || entradaLida == '/')) {
			submaquinaAtual->estadoAtual = 7;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_ARITMETICO);
		}
		else if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 22) {
		submaquinaAtual->estadoAtual = 24;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 23) {
		if ((entradaLida == '+' || entradaLida == '-' || entradaLida == '*' || entradaLida == '/')) {
			submaquinaAtual->estadoAtual = 7;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_OPERADOR_ARITMETICO);
		}
		else if (entradaLida == OPERADOR_OU) {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == '&') {
			submaquinaAtual->estadoAtual = 0;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 24) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
			semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_LE_FECHA_PARENTESES);
		}
	}
	else if (submaquinaAtual->estadoAtual == 25) {
		submaquinaAtual->estadoAtual = 29;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 26) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
		else {
			submaquinaAtual->estadoAtual = 27;
			substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 27) {
		if (entradaLida == ')') {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
		else if (entradaLida == ',') {
			submaquinaAtual->estadoAtual = 28;
			naoEncontrouTransicao = FALSE;
		}
	}
	else if (submaquinaAtual->estadoAtual == 28) {
		submaquinaAtual->estadoAtual = 27;
		substituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido, submaquinaExpressoesCriarSubmaquina());
		naoEncontrouTransicao = FALSE;
	}
	else if (submaquinaAtual->estadoAtual == 29) {
		if (entradaLida == ']') {
			submaquinaAtual->estadoAtual = 23;
			*estaNoEstadoFinal = TRUE;
			naoEncontrouTransicao = FALSE;
		}
	}
	
	if(naoEncontrouTransicao == TRUE & submaquinaAtual->estadoAtual == 0) {
		*algumaSubmaquinaTransitou = FALSE;
		return;
	}
	
	if(naoEncontrouTransicao == TRUE) {
		semantico_tbd(tokenLido, ACAOSEMANTICA_EXPRESSAO_FIM);
		chamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, tokenLido);
	}
	return;	
}
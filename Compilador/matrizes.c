/*
 *  matrizes.c
 *  lexico
 *
 *  Created by Vininicius Oyama on 9/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "matrizes.h"

int** alocarMatrizDeInteiros (int numeroDeLinhas, int numeroDeColunas)
{
	int** matrizASerAlocada; 
	matrizASerAlocada = (int **) calloc (numeroDeLinhas, sizeof(int*));
	
	for ( int indiceLinha = 0; indiceLinha < numeroDeLinhas; indiceLinha++ ) 
		matrizASerAlocada[indiceLinha] = (int*) calloc (numeroDeColunas, sizeof(int));
	
	return matrizASerAlocada;
}
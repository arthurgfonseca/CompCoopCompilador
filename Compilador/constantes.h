//
//  constantes.h
//  Compilador
//
//  Created by Arthur Fonseca on 9/22/11.
//  Copyright 2011 Poli USP. All rights reserved.
//

//
// Valores fixos
//
#define MAX 100000

// Criando a enumeração:
enum boolean {
    true = 1, false = 0
};

typedef  enum boolean  bool;


//
// Definicao dos tipos de dados
//

#define PALAVRA 100
#define INTEIRO 101
#define FLOAT 102
#define DOUBLE 103


#define PALAVRARESERVADA 200
#define OPERADOR 201
#define SIMBOLO 202
#define STRING 203



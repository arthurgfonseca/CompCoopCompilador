/*
 *  submaquinas.h
 *  Compilador
 *
 *  Created by Vininicius Oyama on 10/28/11.
 *  Copyright 2011 Poli USP. All rights reserved.
 *
 */

typedef struct _submaquina {
    int estadoAtual;
	void (*executar)();
} submaquina;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "stack_fp.h"

#define MAXSTACK 100
#define EMPTY_STACK -1

extern int errno;

struct _Stack {
	int top;
	void * item[MAXSTACK];
	P_stack_ele_destroy pf_destroy;
	P_stack_ele_copy pf_copy;
	P_stack_ele_print pf_print;
};

Stack *stack_ini (P_stack_ele_destroy fd, P_stack_ele_copy fc, P_stack_ele_print fp) {
	Stack *s;
	int i;
	s = (Stack*) malloc(sizeof(Stack));
	if (!s) {
		fprintf(stderr, "%s", strerror(errno));
		return NULL;
	}
	// Inicializo tope y asigno los punteros a función
	s->top = -1;
	s->pf_copy = fc;
	s->pf_destroy = fd;
	s->pf_print = fp;
	// Asigno los punteros de los elementos
	for (i=0; i< MAXSTACK; i++)
		s->item[i] = NULL;
		return s;
}

void stack_destroy(Stack* stc) {
	if (!stc) return;
	while (stc->top != EMPTY_STACK) {
		stc->pf_destroy( stc->item[stc->top]); // Llamada a la función cuyo puntero se almacenó como campo
		// de la pila, es decir, utilizando el puntero a la función
		stc->top --;
	}
	free(stc);
}
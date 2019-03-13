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

	/* Inicializo tope y asigno los punteros a función */
	s->top = -1;
	s->pf_copy = fc;
	s->pf_destroy = fd;
	s->pf_print = fp;
	/* Asigno los punteros de los elementos */
	for (i=0; i< MAXSTACK; i++)
		s->item[i] = NULL;
	return s;
}

void stack_destroy(Stack* stc) {
	if (!stc) return;
	while (stc->top != EMPTY_STACK) {
		stc->pf_destroy( stc->item[stc->top]); /* Llamada a la función cuyo puntero se almacenó como campo*/
		/* de la pila, es decir, utilizando el puntero a la función*/
		stc->top --;
	}
	free(stc);
}

/**------------------------------------------------------------------
Inserta un elemento en la pila. Entrada: elemento a insertar y pila donde
insertarlo. Salida: OK si logra insertarlo o ERROR si no.
------------------------------------------------------------------*/
Status stack_push(Stack *stack, const void *ele) {
  void *copia = NULL;

  /*si la pila esta llena no instertamos ningun elemento*/
  if (!stack || !ele || stack_isFull(stack))
    return ERROR;

  copia = stack->pf_copy(ele);
  if (!copia)
    return ERROR;

  /*incrementamos el tope*/
  stack->top++;

  stack->item[stack->top] = copia;

  return OK;
}

/**------------------------------------------------------------------
Extrae un elemento de la pila. Entrada: la pila de donde extraerlo. Salida: NULL
si no logra extraerlo, o el elemento extraído si lo logra. Nótese que la pila
quedará modificada.
------------------------------------------------------------------*/
void *stack_pop(Stack *stack) {
  void *rtEle = NULL;

  if (!stack || stack_isEmpty(stack))
    return NULL;

  rtEle = stack->item[stack->top];

  stack->item[stack->top] = NULL;

  /*decrementamos el tope*/
  stack->top--;

  return rtEle;
}

/**------------------------------------------------------------------
Comprueba si la pila esta vacía. Entrada: puntero a la pila. Salida: TRUE si
está vacía o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isEmpty(const Stack *stack) {
  /*si no existe la pila, se considera vacia para evitar extaccion*/
  if (!stack)
    return TRUE;

  if (stack->top == -1)
    return TRUE;

  return FALSE;
}

/**------------------------------------------------------------------
Comprueba si la pila esta llena. Entrada: puntero a la pila. Salida: TRUE si
está llena o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isFull(const Stack *stack) {
  /*si no existe la pila, se considera llena para evitar insercion*/
  if (!stack)
    return TRUE;

  if (stack->top == MAXSTACK - 1)
    return TRUE;

  return FALSE;
}

/**------------------------------------------------------------------
Imprime toda la pila, comenzando por el último elemento insertado (aquel en la
cima o tope) y terminando por el primero que se insertó. Imprime un elemento por
línea. Entrada: pila y fichero abierto donde imprimir. Salida: número de
caracteres escritos en total.
------------------------------------------------------------------*/
int stack_print(FILE *f, const Stack *stack) {
  int i, impr = 0;

  if (!f || !stack)
    return -1;

  for (i = stack->top; i >= 0; i--)
    impr += stack->pf_print(f, stack->item[i]);

  return impr;
}

#include <stdio.h>
#include <stdlib.h>
#include "stack_elestack.h"

#define MAXSTACK 1024

struct _Stack {
  int top;
  EleStack *item[MAXSTACK];
};


/**------------------------------------------------------------------
Inicializa la pila reservando memoria. Salida: NULL si ha habido error o el
puntero a la pila si ha ido bien
------------------------------------------------------------------*/
Stack *stack_ini() {
  int i;
  Stack *newStack = NULL;

  newStack = (Stack *)calloc(1, sizeof(Stack));
  if (!newStack)
    return NULL;

  for (i = 0; i < MAXSTACK; i++) {
    newStack->item[i] = NULL;
  }

  newStack->top = -1;

  return newStack;
}

/**------------------------------------------------------------------
Elimina la pila Entrada: puntero a la pila que se desea eliminar
------------------------------------------------------------------*/
void stack_destroy(Stack *stack) {
  int i;

  if (stack) {
    for (i = 0; i <= stack->top; i++) {
      EleStack_destroy(stack->item[i]);
      stack->item[i] = NULL;
    }
    free(stack);
  }
}

/**------------------------------------------------------------------
Inserta un elemento en la pila. Entrada: elemento a insertar y pila donde
insertarlo. Salida: OK si logra insertarlo o ERROR si no.
------------------------------------------------------------------*/
Status stack_push(Stack *stack, const EleStack *elestack) {
  EleStack *copia = NULL;

  /*si la pila esta llena no instertamos ningun elemento*/
  if (!stack || !elestack || stack_isFull(stack))
    return ERROR;

  copia = EleStack_copy(elestack);
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
EleStack *stack_pop(Stack *stack) {
  EleStack *rtEle = NULL;

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
    impr += EleStack_print(f, stack->item[i]);

  return impr;
}

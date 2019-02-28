#include <stdio.h>
#include <stdlib.h>
#include "elestack.h"

/*******************************************************************************
EXPLICA EN LA MEMORIA QUE FICH HAS MODIF
******************************************************************************/

struct _EleStack {
	int* e;
};

/**------------------------------------------------------------------
Inicializa un elemento de pila. Salida: Puntero al elemento inicializado o NULL
en caso de error
------------------------------------------------------------------*/
EleStack *EleStack_ini() {

  EleStack *newEle = NULL;

  newEle = (EleStack *)calloc(1, sizeof(EleStack));
  if (!newEle)
    return NULL;

  newEle->e = NULL;

  return newEle;
}

/**------------------------------------------------------------------
Elimina un elemento de pila. Entrada: elemento a destruir.
------------------------------------------------------------------*/
void EleStack_destroy(EleStack *ele) {
  if (ele) {
    if(ele->e) {
			free(ele->e);
		}
    free(ele);
  }
}

/**------------------------------------------------------------------
Modifica los datos de un elemento de pila o EleStack. Entrada: El puntero al
elemento a modificar y el contenido a guardar en dicho elemento. Salida: OK o
ERROR
------------------------------------------------------------------*/
Status EleStack_setInfo(EleStack *ele, void *eleinfo) {
  if (!ele || !eleinfo)
    return ERROR;

	if(ele->e)
  	free(ele->e);

  ele->e = (int *) calloc(1, sizeof(int));
	if (!ele->e)
		return ERROR;

	*(ele->e) = *(int *)eleinfo;
  return OK;
}

/**------------------------------------------------------------------
Devuelve el contenido almacenado en un elemento de pila (del tipo que sea).
Entrada: El elemento de pila. Salida: El contenido de ese elemento, o NULL si ha
habido error.
------------------------------------------------------------------*/
void *EleStack_getInfo(EleStack *ele) {
  if (!ele)
    return NULL;

  return ele->e;
}

/**------------------------------------------------------------------
Copia un elemento de pila en otro, reservando memoria. Entrada: el elemento a
copiar. Salida: puntero al nuevo elemento, copia del recibido, o NULL en caso de
error.
------------------------------------------------------------------*/
EleStack *EleStack_copy(const EleStack *ele) {
  EleStack *newEle = NULL;

  if (!ele)
    return NULL;

  newEle = EleStack_ini();
  if (!newEle)
    return NULL;

	if (EleStack_setInfo(newEle, ele->e) == ERROR)
		return NULL;

  return newEle;
}

/**------------------------------------------------------------------
Compara dos elementos de pila (su contenido). Entrada: los dos elementos a
comparar. Salida: TRUE en caso de ser iguales y FALSE en caso contrario.
------------------------------------------------------------------*/
Bool EleStack_equals(const EleStack *ele1, const EleStack *ele2) {
  /*Si uno de los elementos no existe, consideramos que no son iguales*/
  if (!ele1 || !ele2)
    return FALSE;

  if (*(ele1->e) == *(ele2->e))
    return TRUE;

  return FALSE;
}

/**------------------------------------------------------------------
Imprime un elemento de pila en un fichero ya abierto. Entrada: Fichero en el que
se imprime y elemento a imprimir. Salida: número de caracteres escritos en
total.
------------------------------------------------------------------*/
int EleStack_print(FILE *f, const EleStack *ele) {
  if (!f || !ele)
    return -1;

  return fprintf(f, "[%d]\n", *(ele->e));
}

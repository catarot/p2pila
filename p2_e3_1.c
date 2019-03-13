#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack_fp.h"

/* Especifica como se destruyen los elementos de la pila */
void p_stack_ele_destroy(void *e) {
  node_destroy((Node *)e);
  return;
}

/* Especifica como se copian los elementos de la pila */
void *p_stack_ele_copy(const void *e) {
	return (void *)node_copy((Node *)e);
}

/* Especifica como se muestran por pantalla los elementos de la pila */
int p_stack_ele_print(FILE *f, const void *e) {
  return node_print(f, (Node *)e);
}

int main() {
  Node *node = NULL;
  Stack *stack = NULL;
  int car = 0;

  /*Inicializar un nodo*/
  node = node_ini();
  if (!node) {
    fprintf(stderr, "Error al reservar memoria para el nodo\n");
    return -1;
  }

  /*Inicializar una pila*/
  stack = stack_ini(p_stack_ele_destroy, p_stack_ele_copy, p_stack_ele_print);
  if (!stack) {
    fprintf(stderr, "Error al reservar memoria para la pila\n");
    node_destroy(node);
    return -1;
  }

  /*Asignar al nodo nombre “first” e id 111*/
  node_setId(node, 111);
  node_setName(node, "first");

  /*Introducir una copia del nodo en la pila*/
  if (stack_push(stack, node) == ERROR) {
    fprintf(stderr, "Error al introducir nodo en la pila (1a insercion)\n");
    node_destroy(node);
    stack_destroy(stack);
    return -1;
  }

  /*Cambiar el id y nombre del nodo*/
  node_setId(node, 222);
  node_setName(node, "second");

  /*Introducir una copia del nodo en la pila tras actualizar su info*/
  if (stack_push(stack, node) == ERROR) {
    fprintf(stderr, "Error al introducir nodo en la pila (2a insercion)\n");
    stack_destroy(stack);
    return -1;
  }

  /*Ya he insertado el nodo, puedo eliminarlo*/
  node_destroy(node);

  /*Imprimir estado de la pila. Deberia tener 2 elem: 222(top) y 111*/
  car = stack_print(stdout, stack);
  printf("Caracteres imprimidos: %d\n", car);

  /*Extraccion de todos los elementos de la pila, hasta que quede vacia*/
  printf("Vaciando pila. Extracciones: \n");
  while (!stack_isEmpty(stack)) {
    node = stack_pop(stack);
    if (!node) {
      fprintf(stderr, "Error al extraer de la pila\n");
      stack_destroy(stack);
      return -1;
    }
    node_print(stdout, node);
    /*node se reutiliza en cada iteracion, por lo que hay que liberarlo*/
    node_destroy(node);
  }

  /*Imprimir estado de la pila, que deberia estar vacia*/
  printf("Contenido de la pila despues de vaciar: \n");
  car = stack_print(stdout, stack);
  printf("Caracteres imprimidos: %d\n", car);

  /*Liberar la pila*/
  stack_destroy(stack);

  return 0;
}

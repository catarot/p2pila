#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack_elestack.h"

int main() {
  Node *node = NULL;
  EleStack *ele = NULL;
  Stack *stack = NULL;
  int car = 0;

  /*Inicializar un nodo*/
  node = node_ini();
  if (!node) {
    fprintf(stderr, "Error al reservar memoria para el nodo\n");
    return -1;
  }

  /*Inicializar un elemento de pila*/
  ele = EleStack_ini();
  if (!ele) {
    fprintf(stderr, "Error al reservar memoria para el elemento de pila\n");
    node_destroy(node);
    return -1;
  }

  /*Inicializar una pila*/
  stack = stack_ini();
  if (!stack) {
    fprintf(stderr, "Error al reservar memoria para la pila\n");
    node_destroy(node);
    EleStack_destroy(ele);
    return -1;
  }

  /*Asignar al nodo nombre “first” e id 111*/
  node_setId(node, 111);
  node_setName(node, "first");

  /*Introducir el nodo en un elemento de pila*/
  if (EleStack_setInfo(ele, (void *)node) == ERROR) {
    fprintf(stderr, "Error al introducir nodo en elemento de pila\n");
    node_destroy(node);
    EleStack_destroy(ele);
    stack_destroy(stack);
    return -1;
  }

  /*Introducir una copia del elemento "ele" en la pila*/
  if (stack_push(stack, ele) == ERROR) {
    fprintf(stderr, "Error al introducir ele en la pila (1a insercion)\n");
    node_destroy(node);
    EleStack_destroy(ele);
    stack_destroy(stack);
    return -1;
  }

  /*Cambiar el id y nombre del nodo*/
  node_setId(node, 222);
  node_setName(node, "second");

  /*Actualizar el info del elemento de pila (el nodo que tiene dentro)*/
  if (EleStack_setInfo(ele, node) == ERROR) {
    fprintf(stderr, "Error al actualizar el info del elemento de pila\n");
    node_destroy(node);
    EleStack_destroy(ele);
    stack_destroy(stack);
    return -1;
  }

  /*una copia del nodo ya esta insertada en el elemento, puede liberarse aqui*/
  node_destroy(node);

  /*Introducir una copia de "ele" en la pila tras actualizar su info*/
  if (stack_push(stack, ele) == ERROR) {
    fprintf(stderr, "Error al introducir ele en la pila (2a insercion)\n");
    EleStack_destroy(ele);
    stack_destroy(stack);
    return -1;
  }

  /*una copia de "ele" ya esta insertada en la pila, puede liberarse aqui*/
  EleStack_destroy(ele);

  /*Imprimir estado de la pila. Deberia tener 2 elem: 222(top) y 111*/
  car = stack_print(stdout, stack);
  printf("Caracteres imprimidos: %d\n", car);

  /*Extraccion de todos los elementos de la pila, hasta que quede vacia*/
  printf("Vaciando pila. Extracciones: \n");
  while (!stack_isEmpty(stack)) {
    ele = stack_pop(stack);
    if (!ele) {
      fprintf(stderr, "Error al extraer de la pila\n");
      stack_destroy(stack);
      return -1;
    }
    EleStack_print(stdout, ele);
    /*ele se reutiliza en cada iteracion, por lo que hay que liberarlo*/
    EleStack_destroy(ele);
  }

  /*Imprimir estado de la pila, que deberia estar vacia*/
  printf("Contenido de la pila despues de vaciar: \n");
  car = stack_print(stdout, stack);
  printf("Caracteres imprimidos: %d\n", car);

  /*Liberar la pila*/
  stack_destroy(stack);

  return 0;
}

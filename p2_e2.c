#include <stdio.h>
#include <stdlib.h>
#include "stack_elestack.h"

double media(Stack *stack) {
	double suma = 0, ret = 0;
	int n_elementos = 0;
	EleStack *ele = NULL;
	Stack *stack_aux = NULL;

	if (!stack)
		return -1;

	/*Inicializar una pila auxiliar*/
	stack_aux = stack_ini();
	if (!stack_aux) {
		fprintf(stderr, "Error al reservar memoria para la pila en media\n");
		return -1;
	}

	/* Extraer todos los elementos de la pila, insertandolos en la auxiliar */
	/* Se obtiene el valor del elemento y se almacena en contador */
	while(stack_isEmpty(stack) == FALSE) {
		ele = stack_pop(stack); /* no da error porque pila no vacia */
		stack_push(stack_aux, ele); /* no da error porque acabo de hacer pop */

		suma += *(int *) EleStack_getInfo(ele);

		EleStack_destroy(ele);
		n_elementos++;
	}

	/* Devolver pila a estado inicial */
	while (stack_isEmpty(stack_aux) == FALSE) {
		ele = stack_pop(stack_aux); /* no da error porque pila no vacia */
		stack_push(stack, ele); /* no da error porque acabo de hacer pop */

		EleStack_destroy(ele);
	}

	stack_destroy(stack_aux);
	ret = suma/n_elementos;
	return ret;
}


int main(int argc, char const *argv[]) {
  EleStack *ele = NULL;
  Stack *stack = NULL;
  int pila_total = 0;
	double avg;
	int i;

	if (argc != 2) {
		fprintf(stderr, "Utiliza: ./p2_e2 <num_elementos_de_pila>\n");
		return -1;
	}

	/* El numero de elementos solicitado por terminal se guarda en "pila_total" */
	pila_total = atoi(argv[1]);
	if (pila_total < 0) {
		fprintf(stderr, "Introduce un numero de elementos positivo.\n");
		return -1;
	}

	/*Inicializar una pila*/
	stack = stack_ini();
	if (!stack) {
		fprintf(stderr, "Error al reservar memoria para la pila\n");
		return -1;
	}

	/*Inicializar "pila_total" elementos de pila. Su info es la i del bucle. */
	for (i=0; i <= pila_total; i++) {
	  ele = EleStack_ini();
	  if (!ele) {
	    fprintf(stderr, "Error al reservar memoria para el elemento de pila\n");
	    return -1;
	  }

		if(EleStack_setInfo(ele, &i) == ERROR) {
			fprintf(stderr, "Error al establecer la info del elemento\n");
			EleStack_destroy(ele);
	    return -1;
		}

		if (stack_push(stack, ele) == ERROR) {
	    fprintf(stderr, "Error al introducir ele en la pila (1a insercion)\n");
	    EleStack_destroy(ele);
	    stack_destroy(stack);
	    return -1;
		}

		EleStack_destroy(ele);
	}

	printf("Pila antes de la llamada a la funcion:\n");
	stack_print(stdout, stack);

	avg = media(stack);
	if (avg == -1) {
		fprintf(stderr, "Error en la funcion media\n");
		stack_destroy(stack);
		return -1;
	}

	printf("La media es %f\n", avg);
	stack_print(stdout, stack);

  /*Liberar la pila*/
  stack_destroy(stack);

  return 0;
}

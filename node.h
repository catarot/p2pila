#ifndef NODE_H_
#define NODE_H_

#include "types.h"

typedef struct _Node Node;

/* Inicializa un nodo, reservando memoria y devolviendo el nodo inicializado si
 * lo ha hecho correctamente, sino devuelve NULL en otro caso
 * e imprime el string correspondiente al errorno en stderror */
Node *node_ini();

/* Libera la memoria dinámica reservada para un nodo */
void node_destroy(Node *node);

/* Devuelve el id de un nodo dado, o -1 en caso de error */
int node_getId(const Node *node);

/* Devuelve un puntero al nombre de un nodo dado, o NULL en caso de error */
char *node_getName(const Node *node);

/* Devuelve el número de conexiones de un nodo dado, o -1 en caso de error */
int node_getConnect(const Node *node);

/* Modifica el id de un nodo dado, devuelve NULL en caso de error */
Node *node_setId(Node *node, const int id);

/* Modifica el nombre de un nodo dado, devuelve NULL en caso de error */
Node *node_setName(Node *node, const char *name);

/* Modifica el número de conexiones de un nodo dado, devuelve NULL en caso de
 * error */
Node *node_setConnect(Node *node, const int cn);

/* Compara dos nodos por el id y después el nombre.
 * Devuelve 0 cuando ambos nodos tienen el mismo id, un número menor que
 * 0 cuando n1 < n2 o uno mayor que 0 en caso contrario. */
int node_cmp(const Node *node1, const Node *node2);

/* Reserva memoria para un nodo en el que copia los datos del nodo src.
 * Devuelve la dirección del nodo copia si todo ha ido bien, o NULL en otro
 * caso*/
Node *node_copy(const Node *src);

/* Imprime en pf los datos de un nodo con el formato: [id, name, nConnect]
 * Devuelve el número de caracteres que se han escrito con éxito.
 * Comprueba si ha habido errores en el flujo de salida, en ese caso imprime
 * mensaje de error en stderror*/
int node_print(FILE *pf, const Node *node);

#endif /* NODE_H_ */

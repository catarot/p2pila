#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define MAX_NODES 1064
#define MAX_LINE 256

enum { NO_FILE_POS_VALUES = 2 };

extern int errno;

struct _Graph {
  Node *nodes[MAX_NODES];
  Bool connections[MAX_NODES][MAX_NODES]; /*matriz de adyacencia*/
  int num_nodes;
  int num_edges;
};

/********************************************************************/
/*********************** FUNCIONES PRIVADAS DE GRAPH ****************/
/********************************************************************/
int find_node_index(const Graph *g, int nId1) {
  int i;
  if (!g)
    return -1;
  for (i = 0; i < g->num_nodes; i++) {
    if (node_getId(g->nodes[i]) == nId1)
      return i;
  }

  /* ID not found */
  return -1;
}

int *graph_getConectionsIndex(const Graph *g, int index) {
  int *array = NULL, i, j = 0, size;

  if (!g)
    return NULL;
  if (index < 0 || index > g->num_nodes)
    return NULL;
  /* get memory for the array with the connected nodes index */

  size = node_getConnect(g->nodes[index]);
  array = (int *)malloc(sizeof(int) * size);
  if (!array) {
    /* print errorr message */
    fprintf(stderr, "Error que deberia poner errno.\n");
    return NULL;
  }
  /* asigno valores al array con los indices de los nodos conectados */
  for (i = 0; i < g->num_nodes; i++) {
    if (g->connections[index][i] == TRUE) {
      array[j] = i;
      j++;
    }
  }
  return array;
}

/********************************************************************/
/*********************** FUNCIONES PUBLICAS DE GRAPH ****************/
/********************************************************************/

Status graph_readFromFile(FILE *fin, Graph *g) {
  Node *n;
  char buff[MAX_LINE], name[MAX_LINE];
  int i, nnodes = 0, id1, id2;
  Status flag = ERROR;

  /* read number of nodes */
  if (fgets(buff, MAX_LINE, fin) != NULL)
    if (sscanf(buff, "%d", &nnodes) != 1)
      return ERROR;

  /* init buffer_node */
  n = node_ini();
  if (!n)
    return ERROR;

  /* read nodes line by line */
  for (i = 0; i < nnodes; i++) {
    if (fgets(buff, MAX_LINE, fin) != NULL)
      if (sscanf(buff, "%d %s", &id1, name) != NO_FILE_POS_VALUES)
        break;

    /* set node name and node id */
    node_setName(n, name);
    node_setId(n, id1);

    /* insert node in the graph */
    if (graph_insertNode(g, n) == ERROR)
      break;
  }

  /* Check if all node have been inserted */
  if (i < nnodes) {
    node_destroy(n);
    return ERROR;
  }

  /* read connections line by line and */
  while (fgets(buff, MAX_LINE, fin)) {
    if (sscanf(buff, "%d %d", &id1, &id2) == NO_FILE_POS_VALUES)
      if (graph_insertEdge(g, id1, id2) == ERROR)
        break;
  }

  /* check end of file */
  if (feof(fin))
    flag = OK;

  /* clean up, free resources */
  node_destroy(n);
  return flag;
}

/* Inicializa un grafo */
Graph *graph_ini() {
  Graph *graph = NULL;
  int i, j;

  graph = (Graph *)calloc(1, sizeof(Graph));
  if (graph == NULL) {
    fprintf(stderr, "Error al reservar memoria para el grafo\n");
    return NULL;
  }

  graph->num_nodes = 0;
  graph->num_edges = 0;
  /*No se reserva memoria para los nodos del grafo. Se hará según se inserten */
  for (i = 0; i < MAX_NODES; i++) {
    graph->nodes[i] = NULL;
    for (j = 0; j < MAX_NODES; j++) {
      graph->connections[i][j] = FALSE;
    }
  }

  return graph;
}

/* Libera memoria del grafo */
void graph_destroy(Graph *graph) {
  int i;
  if (graph) {
    for (i = 0; i < (graph->num_nodes); i++) {
      node_destroy(graph->nodes[i]);
			graph->nodes[i] = NULL;
    }
    free(graph);
  }
}

/* Se añade un nodo al grafo (reservando memoria nueva para dicho nodo) siempre
 * y cuando no hubiese ya otro nodo de igual id en el grafo. Actualiza
 * los atributos del grafo que sean necesarios. Devuelve OK o ERROR. */
Status graph_insertNode(Graph *g, const Node *n) {
  int i;

  if (!g || !n)
    return ERROR;

  (g->num_nodes)++;
  /* Si es primera insercion, no hay que comprobar duplicado ni hacer realloc */
  /* Si ya hay nodos en el grafo, entrará en el if */
  if (g->num_nodes > 1) {
    /* Comprueba si hay otro nodo con ese id en el grafo */
    for (i = 0; i < g->num_nodes - 1; i++) {
      if (node_getId(n) == node_getId(g->nodes[i])) {
        /* NOTE: comentario deshabilitado para que cumpla ejemplo ejecucion */
        /* fprintf(stderr, "Error al insertar nodo: id duplicado\n"); */
        (g->num_nodes)--;
        return ERROR;
      }
    }
  }
  /* Crear el nodo y copiar los datos del pasado por argumento al nuevo */
  g->nodes[g->num_nodes - 1] = node_copy(n);

  return OK;
}

/* Se añade una arista entre los nodos de id "nId1" y "nId2".
 * Actualiza los atributos del grafo y de los nodos que sean necesarios.
 * Devuelve OK o ERROR. */
Status graph_insertEdge(Graph *g, const int nId1, const int nId2) {
  int a, b;

  if (!g || nId1 < 0 || nId1 < 0)
    return ERROR;

  a = find_node_index(g, nId1);
  b = find_node_index(g, nId2);
  if (a == -1 || b == -1) {
    fprintf(stderr, "Error al buscar el indice del nodo.\n");
    return ERROR;
  }

  if (g->connections[a][b] == TRUE) {
    fprintf(stderr, "Error: Ya conectados.\n");
    return ERROR;
  }

  (g->num_edges)++;
  g->connections[a][b] = TRUE;

  /* Se incrementa el nConnect del origen */
  b = node_getConnect(g->nodes[a]);
  if (b == -1) {
    fprintf(stderr, "Error al obtener el numero de conexiones del nodo\n");
    (g->num_edges)--;
    return ERROR;
  }
  if (!node_setConnect(g->nodes[a], b + 1)) {
    fprintf(stderr, "Error al modificar el numero de conexiones del nodo\n");
    (g->num_edges)--;
    return ERROR;
  }

  return OK;
}

/* Devuelve una copia del nodo de id "nId" */
Node *graph_getNode(const Graph *g, int nId) {
  int index, connect;
  char *name = NULL;
  Node *newNode = NULL;

  if (!g)
    return NULL;

  newNode = node_ini();

  /* encontrar en el grafo el nodo con la id que me han pasado */
  index = find_node_index(g, nId);
  if (index == -1) {
    fprintf(stderr, "Nodo no encontrado en el grafo.\n");
    return ERROR;
  }

  name = node_getName(g->nodes[index]);
  connect = node_getConnect(g->nodes[index]);
  /*copiar las caracteristicas del nodo encontrado en el grafo en newNode */
  node_setId(newNode, nId);
  node_setName(newNode, name);
  node_setConnect(newNode, connect);

  return newNode;
}

/* Actualiza el nodo del grafo que tiene el mismo id que el nodo n, con la
información de n */
Status graph_setNode(Graph *g, const Node *n) {
  int id, index, connect;
  char *name = NULL;

  if (!g || !n)
    return ERROR;

  id = node_getId(n);
  if (id == -1) {
    fprintf(stderr, "No se puede obtener id de nodo argumento.\n");
    return ERROR;
  }

  index = find_node_index(g, id);
  if (index == -1) {
    fprintf(stderr, "Nodo no encontrado en el grafo.\n");
    return ERROR;
  }

  name = node_getName(n);
  connect = node_getConnect(n);
  node_setName(g->nodes[index], name);
  node_setConnect(g->nodes[index], connect);

  return OK;
}

/* Devuelve la dirección de un array con los id de todos los nodos del grafo.
 * Reserva memoria para el array. */
int *graph_getNodesId(const Graph *g) {
  int *all_id = NULL;
  int i;

  if (!g)
    return NULL; /*comprobar los returns*/

  all_id = (int *)calloc(MAX_NODES, sizeof(int));

  if (!all_id)
    return NULL;

  for (i = 0; i < MAX_NODES; i++) {
    if (node_getId(g->nodes[i]) == -1)
      return NULL;
  }

  return all_id;
}

/* Devuelve el número de nodos del grafo. -1 si ha habido algún error*/
int graph_getNumberOfNodes(const Graph *g) {
  if (!g)
    return -1;

  return g->num_nodes;
}

/* Devuelve el número de aristas del grafo. -1 si ha habido algún error*/
int graph_getNumberOfEdges(const Graph *g) {
  if (!g)
    return -1;

  return g->num_edges;
}

/* Determina si dos nodos están conectados*/
Bool graph_areConnected(const Graph *g, const int nId1, const int nId2) {
  int a, b;

  if (!g || nId1 < 0 || nId2 < 0)
    return FALSE;

  a = find_node_index(g, nId1);
  b = find_node_index(g, nId2);
  if (a == -1 || b == -1) {
    fprintf(stderr, "Error al buscar el indice del nodo.\n");
    return FALSE;
  }

  return g->connections[a][b];
}

/* Devuelve el número de conexiones del nodo de id fromId */
int graph_getNumberOfConnectionsFrom(const Graph *g, const int fromId) {
  int a;

  if (!g || fromId < 0)
    return -1;

  a = find_node_index(g, fromId);
  if (a == -1)
    return -1;

  return node_getConnect(g->nodes[a]);
}

/* Devuelve la dirección de un array con los id de todos los nodos del grafo.
 * Reserva memoria para el array. */
int *graph_getConnectionsFrom(const Graph *g, const int fromId) {
  int index;

  index = find_node_index(g, fromId);
  /* si hay error en find_node_index, devuelve -1. Al pasar -1 a
	graph_getConectionsIndex, devuelve NULL por error de argumentos */
  return graph_getConectionsIndex(g, index);
}

/* Imprime en el flujo pf los datos de un grafo, devolviendo el número de
caracteres impresos.* Comprueba si ha habido errorres en el flujo de salida. Si
es así imprime mensaje
* de error en stderror y devuelve el valor -1. ->ok
* El formato a seguir es: imprimir una linea por nodo con la información
asociada al nodo y
* los id de sus conexiones. La salida para el grafo del ejercicio 2.3 de la
parte 1 es:
* [1, a, 2] 2 3
* [2, b, 2] 1 3
* [3, c, 2]] 1 2 */
int graph_print(FILE *pf, const Graph *g) {
  int ret = 0;
  int i, j, a, node_id, n_id, n_connected;
  char *n_name = NULL;
  int *conn = NULL;

  if (!pf || !g)
    return -1;

  for (i = 0; i < g->num_nodes; i++) {
    n_id = node_getId(g->nodes[i]);
    n_name = node_getName(g->nodes[i]);
    n_connected = node_getConnect(g->nodes[i]);

    ret += fprintf(pf, "[%d, %s, %d]", n_id, n_name, n_connected);

    a = find_node_index(g, node_getId(g->nodes[i]));
    conn = graph_getConectionsIndex(g, a);
    for (j = 0; j < node_getConnect(g->nodes[i]); j++) {
      node_id = node_getId(g->nodes[conn[j]]);
      ret += fprintf(pf, " %d", node_id);
    }
    fprintf(pf, "\n");

    free(conn);
  }

  if (ret < 1) {
    fprintf(stderr, "Error al imprimir el grafo\n");
    return -1;
  }

  return ret;
}

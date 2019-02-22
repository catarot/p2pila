#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

struct _Node {
  char name[100];
  int id;
  int nConnect;
};

Node *node_ini() {
  Node *node = NULL;

  node = (Node *)calloc(1, sizeof(Node));
  if (node == NULL) {
    fprintf(stderr, "Error al reservar memoria para el nodo\n");
    return NULL;
  }

  node->name[0] = '\0';
  node->id = 0;
  node->nConnect = 0;

  return node;
}

void node_destroy(Node *node) {
  if (node)
    free(node);
}

int node_getId(const Node *node) {
  if (!node)
    return -1;

  return node->id;
}

char *node_getName(const Node *node) {
	if (!node)
    return NULL;

  return ((char *) node->name);
}

int node_getConnect(const Node *node) {
  if (!node)
    return -1;

  return node->nConnect;
}

Node *node_setId(Node *node, const int id) {
  if(!node || id < 1)
		return NULL;

  node->id = id;
  return node;
}

Node *node_setName(Node *node, const char *name) {
  if (!node || !name)
    return NULL;

  strcpy(node->name, name);
  return node;
}

Node *node_setConnect(Node *node, const int cn) {
  if (!node || cn < 0)
    return NULL;

  node->nConnect = cn;
  return node;
}

int node_cmp(const Node *node1, const Node *node2) {
  if (!node1 || !node2)
    return 1;

  if (node1->id == node2->id)
    return 0;
  else if (node1->id < node2->id)
    return -1;
  else
    return 1;
}

Node *node_copy(const Node *src) {
  Node *newNode = NULL;

  if (!src)
    return NULL;

  newNode = (Node *)calloc(1, sizeof(Node));
  if (!newNode) {
    fprintf(stderr, "Error al reservar memoria para el nodo copia\n");
    return NULL;
  }

  newNode->id = src->id;
  strcpy(newNode->name, src->name);
  newNode->nConnect = src->nConnect;

  return newNode;
}

int node_print(FILE *pf, const Node *node) {
	int ret = -1;

  if (!pf || !node)
    return -1;

	ret = fprintf(pf, "[%d, %s, %d]\n",node->id, (char*)(node->name), node->nConnect);
  if(ret < 1){
		fprintf(stderr, "Error al imprimir el nodo\n");
    return -1;
  }

  return ret;
}

#include	<stdlib.h>
#include	<stdio.h>
#include	<assert.h>

#ifndef _EXPRESSION_TREE_HANDLE_
#define _EXPRESSION_TREE_HANDLE_

typedef struct NodesDesc *Node;

typedef struct NodeDesc{
	char kind;			// plus , minus , times , divide , number
	int val;			// nummber: value
	Node left, right;	// plus, mins times, divde: children
} NodeDesc

#endif

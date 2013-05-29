#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"

#define REALLOC_CHUNK_MATCHSTR 8

#define REALLOC_CHUNK_OUTGOING 8

void node_init         (AC_NODE_t * thiz);
int  node_edge_compare (const void * l, const void * r);
int  node_has_matchstr (AC_NODE_t * thiz, AC_PATTERN_t * newstr);

struct node * node_create(void)
{
	AC_NODE_t * thiz;
	thiz = (AC_NODE_t *) malloc (sizeof(AC_NODE_t));
	node_init(thiz);
	node_assign_id(thiz);
	return thiz;
}

void node_init(AC_NODE_t * thiz)
{
	memset(thiz, 0, sizeof(AC_NODE_t));

	thiz->outgoing_max = REALLOC_CHUNK_OUTGOING;
	thiz->outgoing = (struct edge *) malloc
			(thiz->outgoing_max*sizeof(struct edge));

	thiz->matched_patterns_max = REALLOC_CHUNK_MATCHSTR;
	thiz->matched_patterns = (AC_PATTERN_t *) malloc
			(thiz->matched_patterns_max*sizeof(AC_PATTERN_t));
}

void node_release(AC_NODE_t * thiz)
{
	free(thiz->matched_patterns);
	free(thiz->outgoing);
	free(thiz);
}

AC_NODE_t * node_find_next(AC_NODE_t * thiz, AC_ALPHABET_t alpha)
{
	int i;

	for (i=0; i < thiz->outgoing_degree; i++)
	{
		if(thiz->outgoing[i].alpha == alpha)
			return (thiz->outgoing[i].next);
	}
	return NULL;
}

AC_NODE_t * node_findbs_next (AC_NODE_t * thiz, AC_ALPHABET_t alpha)
{
	int min, max, mid;
	AC_ALPHABET_t amid;

	min = 0;
	max = thiz->outgoing_degree - 1;

	while (min <= max)
	{
		mid = (min+max) >> 1;
		amid = thiz->outgoing[mid].alpha;
		if (alpha > amid)
			min = mid + 1;
		else if (alpha < amid)
			max = mid - 1;
		else
			return (thiz->outgoing[mid].next);
	}
	return NULL;
}

int node_has_matchstr (AC_NODE_t * thiz, AC_PATTERN_t * newstr)
{
	int i, j;
	AC_PATTERN_t * str;

	for (i=0; i < thiz->matched_patterns_num; i++)
	{
		str = &thiz->matched_patterns[i];

		if (str->length != newstr->length)
			continue;

		for (j=0; j<str->length; j++)
			if(str->astring[j] != newstr->astring[j])
				continue;

		if (j == str->length)
			return 1;
	}
	return 0;
}

AC_NODE_t * node_create_next (AC_NODE_t * thiz, AC_ALPHABET_t alpha)
{
	AC_NODE_t * next;
	next = node_find_next (thiz, alpha);
	if (next)
	/* The edge already exists */
		return NULL;
	/* Otherwise register new edge */
	next = node_create ();
	node_register_outgoing(thiz, next, alpha);

	return next;
}

void node_register_matchstr (AC_NODE_t * thiz, AC_PATTERN_t * str)
{
	/* Check if the new pattern already exists in the node list */
	if (node_has_matchstr(thiz, str))
		return;

	/* Manage memory */
	if (thiz->matched_patterns_num >= thiz->matched_patterns_max)
	{
		thiz->matched_patterns_max += REALLOC_CHUNK_MATCHSTR;
		thiz->matched_patterns = (AC_PATTERN_t *) realloc 
			(thiz->matched_patterns, thiz->matched_patterns_max*sizeof(AC_PATTERN_t));
	}

	thiz->matched_patterns[thiz->matched_patterns_num].astring = str->astring;
	thiz->matched_patterns[thiz->matched_patterns_num].length = str->length;
	thiz->matched_patterns[thiz->matched_patterns_num].rep = str->rep;
	thiz->matched_patterns_num++;
}

void node_register_outgoing
	(AC_NODE_t * thiz, AC_NODE_t * next, AC_ALPHABET_t alpha)
{
	if(thiz->outgoing_degree >= thiz->outgoing_max)
	{
		thiz->outgoing_max += REALLOC_CHUNK_OUTGOING;
		thiz->outgoing = (struct edge *) realloc 
			(thiz->outgoing, thiz->outgoing_max*sizeof(struct edge));
	}

	thiz->outgoing[thiz->outgoing_degree].alpha = alpha;
	thiz->outgoing[thiz->outgoing_degree++].next = next;
}

void node_assign_id (AC_NODE_t * thiz)
{
	static int unique_id = 1;
	thiz->id = unique_id ++;
}

int node_edge_compare (const void * l, const void * r)
{
	if ( ((struct edge *)l)->alpha >= ((struct edge *)r)->alpha )
		return 1;
	else
		return -1;
}

void node_sort_edges (AC_NODE_t * thiz)
{
	qsort ((void *)thiz->outgoing, thiz->outgoing_degree, sizeof(struct edge),
			node_edge_compare);
}
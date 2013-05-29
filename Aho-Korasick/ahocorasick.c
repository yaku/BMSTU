#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ahocorasick.h"

/* Allocation step for automata.all_nodes */
#define REALLOC_CHUNK_ALLNODES 200

/* Private function prototype */
static void ac_register_nodeptr
	(AC_t * thiz, AC_NODE_t * node);
static void ac_union_matchstrs
	(AC_NODE_t * node);
static void ac_set_failure
	(AC_t * thiz, AC_NODE_t * node, AC_ALPHABET_t * alphas);
static void ac_traverse_setfailure
	(AC_t * thiz, AC_NODE_t * node, AC_ALPHABET_t * alphas);

AC_t * ac_init (MATCH_CALBACK_f mc)
{
	AC_t * thiz = (AC_t *)malloc(sizeof(AC_t));
	memset (thiz, 0, sizeof(AC_t));
	thiz->root = node_create ();
	thiz->all_nodes_max = REALLOC_CHUNK_ALLNODES;
	thiz->all_nodes = (AC_NODE_t **) malloc (thiz->all_nodes_max*sizeof(AC_NODE_t *));
	thiz->match_callback = mc;
	ac_register_nodeptr (thiz, thiz->root);
	ac_reset (thiz);
	thiz->total_patterns = 0;
	thiz->automata_open = 1;
	return thiz;
}

AC_ERROR_t ac_add (AC_t * thiz, AC_PATTERN_t * patt)
{
	unsigned int i;
	AC_NODE_t * n = thiz->root;
	AC_NODE_t * next;
	AC_ALPHABET_t alpha;

	if(!thiz->automata_open)
		return ACERR_CLOSED;

	if (!patt->length)
		return ACERR_ZERO_PATTERN;

	if (patt->length > AC_PATTRN_MAX_LENGTH)
		return ACERR_LONG_PATTERN;

	for (i=0; i<patt->length; i++)
	{
		alpha = patt->astring[i];
		if ((next = node_find_next(n, alpha)))
		{
			n = next;
			continue;
		}
		else
		{
			next = node_create_next(n, alpha);
			next->depth = n->depth + 1;
			n = next;
			ac_register_nodeptr(thiz, n);
		}
	}

	if(n->final)
		return ACERR_DUPLICATE_PATTERN;

	n->final = 1;
	node_register_matchstr(n, patt);
	thiz->total_patterns++;

	return ACERR_SUCCESS;
}

void ac_finalize (AC_t * thiz)
{
	unsigned int i;
	AC_ALPHABET_t alphas[AC_PATTRN_MAX_LENGTH];
	AC_NODE_t * node;

	ac_traverse_setfailure (thiz, thiz->root, alphas);

	for (i=0; i < thiz->all_nodes_num; i++)
	{
		node = thiz->all_nodes[i];
		ac_union_matchstrs (node);
		node_sort_edges (node);
	}
	thiz->automata_open = 0; /* do not accept patterns any more */
}

int ac_search (AC_t * thiz, AC_TEXT_t * txt, void * param)
{
	unsigned long position;
	AC_NODE_t * current;
	AC_NODE_t * next;

	if(thiz->automata_open)
		/* you must call ac_locate_failure() first */
		return -1;

	position = 0;
	current = thiz->current_node;

	/* This is the main search loop.
	 * it must be keep as lightweight as possible. */
	while (position < txt->length)
	{
		if(!(next = node_findbs_next(current, txt->astring[position])))
		{
			if(current->failure_node /* we are not in the root node */)
				current = current->failure_node;
			else
				position++;
		}
		else
		{
			current = next;
			position++;
		}

		if(current->final && next)

		{
			thiz->match.position = position + thiz->base_position;
			thiz->match.match_num = current->matched_patterns_num;
			thiz->match.patterns = current->matched_patterns;
			/* we found a match! do call-back */
			if (thiz->match_callback(&thiz->match, param))
				return 1;
		}
	}

	/* save status variables */
	thiz->current_node = current;
	thiz->base_position += position;
	return 0;
}


void ac_reset (AC_t * thiz)
{
	thiz->current_node = thiz->root;
	thiz->base_position = 0;
}

void ac_release (AC_t * thiz)
{
	unsigned int i;
	AC_NODE_t * n;

	for (i=0; i < thiz->all_nodes_num; i++)
	{
		n = thiz->all_nodes[i];
		node_release(n);
	}
	free(thiz->all_nodes);
	free(thiz);
}

void ac_display (AC_t * thiz, char repcast)
{
	unsigned int i, j;
	AC_NODE_t * n;
	struct edge * e;
	AC_PATTERN_t sid;

	printf("---------------------------------\n");

	for (i=0; i<thiz->all_nodes_num; i++)
	{
		n = thiz->all_nodes[i];
		printf("NODE(%3d)/----fail----> NODE(%3d)\n",
				n->id, (n->failure_node)?n->failure_node->id:1);
		for (j=0; j<n->outgoing_degree; j++)
		{
			e = &n->outgoing[j];
			printf("         |----(");
			if(isgraph(e->alpha))
				printf("%c)---", e->alpha);
			else
				printf("0x%x)", e->alpha);
			printf("--> NODE(%3d)\n", e->next->id);
		}
		if (n->matched_patterns_num) {
			printf("Accepted patterns: {");
			for (j=0; j<n->matched_patterns_num; j++)
			{
				sid = n->matched_patterns[j];
				if(j) printf(", ");
				switch (repcast)
				{
				case 'n':
					printf("%ld", sid.rep.number);
					break;
				case 's':
					printf("%s", sid.rep.stringy);
					break;
				}
			}
			printf("}\n");
		}
		printf("---------------------------------\n");
	}
}

static void ac_register_nodeptr (AC_t * thiz, AC_NODE_t * node)
{
	if(thiz->all_nodes_num >= thiz->all_nodes_max)
	{
		thiz->all_nodes_max += REALLOC_CHUNK_ALLNODES;
		thiz->all_nodes = realloc
				(thiz->all_nodes, thiz->all_nodes_max*sizeof(AC_NODE_t *));
	}
	thiz->all_nodes[thiz->all_nodes_num++] = node;
}

static void ac_union_matchstrs (AC_NODE_t * node)
{
	unsigned int i;
	AC_NODE_t * m = node;

	while ((m = m->failure_node))
	{
		for (i=0; i < m->matched_patterns_num; i++)
			node_register_matchstr(node, &(m->matched_patterns[i]));

		if (m->final)
			node->final = 1;
	}
}

static void ac_set_failure
	(AC_t * thiz, AC_NODE_t * node, AC_ALPHABET_t * alphas)
{
	unsigned int i, j;
	AC_NODE_t * m;

	for (i=1; i < node->depth; i++)
	{
		m = thiz->root;
		for (j=i; j < node->depth && m; j++)
			m = node_find_next (m, alphas[j]);
		if (m)
		{
			node->failure_node = m;
			break;
		}
	}
	if (!node->failure_node)
		node->failure_node = thiz->root;
}

static void ac_traverse_setfailure
	(AC_t * thiz, AC_NODE_t * node, AC_ALPHABET_t * alphas)
{
	unsigned int i;
	AC_NODE_t * next;

	for (i=0; i < node->outgoing_degree; i++)
	{
		alphas[node->depth] = node->outgoing[i].alpha;
		next = node->outgoing[i].next;

		ac_set_failure (thiz, next, alphas);
		ac_traverse_setfailure (thiz, next, alphas);
	}
}

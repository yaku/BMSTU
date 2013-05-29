#include "node.h"

typedef struct
{
	/* The root of the Aho-Corasick trie */
	AC_NODE_t * root;

	AC_NODE_t ** all_nodes;

	unsigned int all_nodes_num; /* Number of all nodes in the automata */
	unsigned int all_nodes_max; /* Current max allocated memory for *all_nodes */

	AC_MATCH_t match; /* Any match is reported with this */
	MATCH_CALBACK_f match_callback; /* Match call-back function */

	unsigned short automata_open;


	AC_NODE_t * current_node; /* Pointer to current node while searching */
	unsigned long base_position; /* Represents the position of current chunk
	related to whole input text */

	unsigned long total_patterns; /* Total patterns in the automata */

} AC_t;


AC_t * ac_init     (MATCH_CALBACK_f mc);
AC_ERROR_t      ac_add      (AC_t * thiz, AC_PATTERN_t * str);
void            ac_finalize (AC_t * thiz);
int             ac_search   (AC_t * thiz, AC_TEXT_t * str, void * param);
void            ac_reset    (AC_t * thiz);
void            ac_release  (AC_t * thiz);
void            ac_display  (AC_t * thiz, char repcast);
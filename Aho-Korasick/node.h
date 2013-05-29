typedef char AC_ALPHABET_t;

typedef union {
	char * stringy; /* null-terminated string */
	unsigned long number;
} AC_REP_t;

typedef struct
{
	AC_ALPHABET_t * astring; /* String of alphabets */
	unsigned int length; /* Length of pattern */
	AC_REP_t rep; /* Representative string (optional) */
} AC_PATTERN_t;

typedef struct
{
	AC_ALPHABET_t * astring; /* String of alphabets */
	unsigned int length; /* Length of string */
} AC_TEXT_t;

typedef struct
{
	AC_PATTERN_t * patterns; /* Array of matched pattern */
	long position; /* The end position of matching pattern(s) in the text */
	unsigned int match_num; /* Number of matched patterns */
} AC_MATCH_t;

typedef enum
{
	ACERR_SUCCESS = 0,
	ACERR_DUPLICATE_PATTERN, 
	ACERR_LONG_PATTERN, 
	ACERR_ZERO_PATTERN, 
	ACERR_CLOSED, 
	
} AC_ERROR_t;

typedef int (*MATCH_CALBACK_f)(AC_MATCH_t *, void *);

#define AC_PATTRN_MAX_LENGTH 1024

struct edge;

typedef struct node
{
	int id; /* Node ID : for debugging purpose */
	short int final; /* 0: no ; 1: yes, it is a final node */
	struct node * failure_node; /* The failure node of this node */
	unsigned short depth; /* depth: distance between this node and the root */

	/* Matched patterns */
	AC_PATTERN_t * matched_patterns; /* Array of matched patterns */
	unsigned short matched_patterns_num; /* Number of matched patterns at this node */
	unsigned short matched_patterns_max; /* Max capacity of allocated memory for matched_patterns */

	/* Outgoing Edges */
	struct edge * outgoing; /* Array of outgoing edges */
	unsigned short outgoing_degree; /* Number of outgoing edges */
	unsigned short outgoing_max; /* Max capacity of allocated memory for outgoing */
} AC_NODE_t;

struct edge
{
	AC_ALPHABET_t alpha; /* Edge alpha */
	struct node * next; /* Target of the edge */
};

AC_NODE_t * node_create            (void);
AC_NODE_t * node_create_next       (AC_NODE_t * thiz, AC_ALPHABET_t alpha);
void        node_register_matchstr (AC_NODE_t * thiz, AC_PATTERN_t * str);
void        node_register_outgoing (AC_NODE_t * thiz, AC_NODE_t * next, AC_ALPHABET_t alpha);
AC_NODE_t * node_find_next         (AC_NODE_t * thiz, AC_ALPHABET_t alpha);
AC_NODE_t * node_findbs_next       (AC_NODE_t * thiz, AC_ALPHABET_t alpha);
void        node_release           (AC_NODE_t * thiz);
void        node_assign_id         (AC_NODE_t * thiz);
void        node_sort_edges        (AC_NODE_t * thiz);


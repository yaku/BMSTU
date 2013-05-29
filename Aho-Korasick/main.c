#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "ahocorasick.h"

AC_ALPHABET_t * allstr[] = {
	"rec",
	"cent",
	"ece",
	"ce",
	"recent",
	"nt",
};

#define PATTERN_NUMBER (sizeof(allstr)/sizeof(AC_ALPHABET_t *))

/* Input text */
AC_ALPHABET_t * input_text = {"She recently graduated from college"};

int match_handler(AC_MATCH_t * m, void * param)
{
	unsigned int j;

	printf ("@ %ld : ", m->position);

	for (j=0; j < m->match_num; j++)
		printf("%ld (%s), ", m->patterns[j].rep.number, m->patterns[j].astring);

	printf("\n");

	return 0;
}

int main (int argc, char ** argv)
{
	unsigned int i;

	AC_t * acap;
	AC_PATTERN_t tmp_patt;
	AC_TEXT_t tmp_text;

	acap = ac_init (match_handler);

	for (i=0; i<PATTERN_NUMBER; i++)
	{
		tmp_patt.astring = allstr[i];
		tmp_patt.rep.number = i+1;
		tmp_patt.length = strlen(tmp_patt.astring);
		ac_add (acap, &tmp_patt);
	}

	ac_finalize (acap);

	tmp_text.astring = input_text;
	tmp_text.length = strlen(tmp_text.astring);

	ac_search (acap, &tmp_text, 0);
	ac_reset(acap);
	ac_release (acap);

	getch();
	return 0;
}
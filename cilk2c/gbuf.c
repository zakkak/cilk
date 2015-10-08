#include <basics.h>
//#include <assert.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

GLOBAL int gbprintf(GBUF *gb, const char *format, ...) {
    int r;
    va_list ap;
    va_start(ap, format);
    r=gb->vfprintf_fun(gb->state, format, ap);
    va_end(ap);
    return r;
}
GLOBAL int gbputs (const char *string, GBUF *gb) {
    return gb->fputs_fun(string, gb->state);
}

/*********************************************
 * functions for creating output to a string.
 *********************************************/

struct mbuffer {
    char *string;
    char *tail;
    int  n_chars_left; /* Number of characters left in the buffer (including the tailing \0) */
};

PRIVATE void init_mbuffer (char *array, int arraysize, struct mbuffer *b) {
    b->string=array;
    b->tail  =array;
    assert(arraysize>0);
    b->n_chars_left=arraysize;
    array[0]=0;
}

/* Act like fputs. */
PRIVATE int add_string_to_mbuffer(const char *string, void *state) {
    struct mbuffer *b=state;
    int l = strlen(string);
    if (l+1>b->n_chars_left) return -1;
    strcpy(b->tail, string); /* Don't use strncpy, because that fills in the whole tail of the dest with nulls. Slow and error prone. */
    b->tail+=l;
    b->n_chars_left-=l;
    return l;
}

PRIVATE void free_mbuffer (void *s) {
    free(s);
}

GLOBAL int vfprintf_to_mbuffer (void *state, const char *format, va_list ap) {
    struct mbuffer *mb = state;
    int    oldavail  = mb->n_chars_left;
    int    n_written = vsnprintf(mb->tail, oldavail, format, ap);
    if (n_written>=oldavail || n_written<0) {
	mb->tail[oldavail-1]=0;
	mb->tail+=oldavail;
	mb->n_chars_left=0;
	return -1;
    } else {
	mb->tail+=n_written;
	mb->n_chars_left-=n_written;
	return n_written;
    }
}


GLOBAL GBUF* make_string_gbuf (char *array, int arraysize) {
    GBUF *result = malloc(sizeof(*result));
    struct mbuffer *mb = malloc(sizeof(*mb));
    result->state = mb;
    result->fputs_fun = add_string_to_mbuffer;
    result->vfprintf_fun = vfprintf_to_mbuffer;
    result->free_state_fun = free_mbuffer;
    init_mbuffer(array, arraysize, mb);
    return result;
}

PRIVATE int add_string_to_file (const char *string, void *state) {
    FILE *f=state;
    return fputs(string, f);
}

PRIVATE int vfprintf_to_file (void *state, const char *format, va_list ap) {
    FILE *stream=state;
    return vfprintf(stream, format, ap);
}

GLOBAL GBUF* make_file_gbuf (FILE *f) {
    GBUF *result = malloc(sizeof(*result));
    result->state = f;
    result->fputs_fun = add_string_to_file;
    result->vfprintf_fun = vfprintf_to_file;
    result->free_state_fun = 0;
    return result;
}

GLOBAL void free_gbuf (GBUF *gb) {
    if (gb->free_state_fun) gb->free_state_fun(gb->state);
    free(gb);
}


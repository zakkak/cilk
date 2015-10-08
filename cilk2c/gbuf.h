#include <stdarg.h>
#include <stdio.h>

typedef struct generalized_buffer {
    void *state;
    int (*fputs_fun)(const char *string, void *state);
    int (*vfprintf_fun)(void *state, const char *format, va_list);
    void (*free_state_fun)(void*);
} GBUF;

GLOBAL int gbprintf(GBUF *gb, const char *format, ...) __attribute__((format (printf, 2, 3)));
GLOBAL int gbputs (const char *string, GBUF *gb);     

GLOBAL GBUF* make_string_gbuf (char *array, int arraysize);
GLOBAL GBUF* make_file_gbuf (FILE *f);
GLOBAL void  free_gbuf(GBUF*);

#define WITH_FILE_GBUF(gb,file,stmt) ({ GBUF *gb = make_file_gbuf(file); stmt; free_gbuf(gb); })

/* SCHEME.H */

#ifndef _SCHEME_H
#define _SCHEME_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Default values for #define'd symbols
 */
#ifndef STANDALONE       /* If used as standalone interpreter */
# define STANDALONE 0
#else
# define STANDALONE 1
#endif

#ifndef _WIN32
# define USE_STRCASECMP 1
# ifndef USE_STRLWR
#   define USE_STRLWR 1
# endif
# define SCHEME_EXPORT
#else
# define USE_STRCASECMP 0
# define USE_STRLWR 0
# ifdef _SCHEME_SOURCE
#  define SCHEME_EXPORT __declspec(dllexport)
# else
#  define SCHEME_EXPORT __declspec(dllimport)
# endif
#endif

#if USE_NO_FEATURES
# define USE_MATH 0
# define USE_CHAR_CLASSIFIERS 0
# define USE_ASCII_NAMES 0
# define USE_STRING_PORTS 0
# define USE_ERROR_HOOK 0
# define USE_TRACING 0
# define USE_COLON_HOOK 0
# define USE_DL 0
# define USE_PLIST 0
#endif

/*
 * Leave it defined if you want continuations, and also for the Sharp Zaurus.
 * Undefine it if you only care about faster speed and not strict Scheme compatibility.
 */
#define USE_SCHEME_STACK

#if USE_DL
# define USE_INTERFACE 1
#endif


#ifndef USE_MATH         /* If math support is needed */
# define USE_MATH 1
#endif

#ifndef USE_CHAR_CLASSIFIERS  /* If char classifiers are needed */
# define USE_CHAR_CLASSIFIERS 1
#endif

#ifndef USE_ASCII_NAMES  /* If extended escaped characters are needed */
# define USE_ASCII_NAMES 1
#endif

#ifndef USE_STRING_PORTS      /* Enable string ports */
# define USE_STRING_PORTS 1
#endif

#ifndef USE_TRACING
# define USE_TRACING 1
#endif

#ifndef USE_PLIST
# define USE_PLIST 0
#endif

/* To force system errors through user-defined error handling (see *error-hook*) */
#ifndef USE_ERROR_HOOK
# define USE_ERROR_HOOK 1
#endif

#ifndef USE_COLON_HOOK   /* Enable qualified qualifier */
# define USE_COLON_HOOK 1
#endif

#ifndef USE_STRCASECMP   /* stricmp for Unix */
# define USE_STRCASECMP 0
#endif

#ifndef USE_STRLWR
# define USE_STRLWR 1
#endif

#ifndef STDIO_ADDS_CR    /* Define if DOS/Windows */
# define STDIO_ADDS_CR 0
#endif

#ifndef INLINE
# define INLINE
#endif

#ifndef USE_INTERFACE
# define USE_INTERFACE 1
#endif

#ifndef SHOW_ERROR_LINE   /* Show error line in file */
# define SHOW_ERROR_LINE 1
#endif

typedef struct scheme scheme;
typedef struct cell *pointer;

typedef void * (*func_alloc)(size_t);
typedef void (*func_dealloc)(void *);

/* num, for generic arithmetic */
typedef struct num {
     char is_fixnum;
     union {
          long ivalue;
          double rvalue;
     } value;
} num;

SCHEME_EXPORT scheme* scheme_init_new();
SCHEME_EXPORT scheme* scheme_init_new_custom_alloc(func_alloc malloc, func_dealloc free);
SCHEME_EXPORT int scheme_init(scheme *sc);
SCHEME_EXPORT int scheme_init_custom_alloc(scheme *sc, func_alloc, func_dealloc);
SCHEME_EXPORT void scheme_deinit(scheme *sc);
void scheme_set_input_port_file(scheme *sc, FILE *fin);
void scheme_set_input_port_string(scheme *sc, char *start, char *past_the_end);
SCHEME_EXPORT void scheme_set_output_port_file(scheme *sc, FILE *fin);
void scheme_set_output_port_string(scheme *sc, char *start, char *past_the_end);
SCHEME_EXPORT void scheme_load_file(scheme *sc, FILE *fin);
SCHEME_EXPORT void scheme_load_named_file(scheme *sc, FILE *fin, const char *filename);
SCHEME_EXPORT void scheme_load_string(scheme *sc, const char *cmd);
SCHEME_EXPORT pointer scheme_apply0(scheme *sc, const char *procname);
SCHEME_EXPORT pointer scheme_call(scheme *sc, pointer func, pointer args);
SCHEME_EXPORT pointer scheme_eval(scheme *sc, pointer obj);
void scheme_set_external_data(scheme *sc, void *p);
SCHEME_EXPORT void scheme_define(scheme *sc, pointer env, pointer symbol, pointer value);

SCHEME_EXPORT pointer scm_load_ext(scheme *sc, pointer arglist);

typedef pointer (*foreign_func)(scheme*, pointer);

pointer _cons(scheme* sc, pointer a, pointer b, int immutable);
pointer mk_integer(scheme* sc, long num);
pointer mk_real(scheme* sc, double num);
pointer mk_symbol(scheme* sc, const char *name);
pointer gensym(scheme* sc);
pointer mk_string(scheme* sc, const char *str);
pointer mk_counted_string(scheme* sc, const char *str, int len);
pointer mk_empty_string(scheme* sc, int len, char fill);
pointer mk_character(scheme* sc, int c);
pointer mk_foreign_func(scheme* sc, foreign_func f);
void putstr(scheme* sc, const char* s);
int list_length(scheme* sc, pointer a);
int eqv(pointer a, pointer b);


#if USE_INTERFACE

struct scheme_interface {
     void (*scheme_define)(scheme *sc, pointer env, pointer symbol, pointer value);
     pointer (*cons)(scheme *sc, pointer a, pointer b);
     pointer (*immutable_cons)(scheme *sc, pointer a, pointer b);
     pointer (*reserve_cells)(scheme *sc, int n);
     pointer (*mk_integer)(scheme *sc, long num);
     pointer (*mk_real)(scheme *sc, double num);
     pointer (*mk_symbol)(scheme *sc, const char *name);
     pointer (*gensym)(scheme *sc);
     pointer (*mk_string)(scheme *sc, const char *str);
     pointer (*mk_counted_string)(scheme *sc, const char *str, int len);
     pointer (*mk_character)(scheme *sc, int c);
     pointer (*mk_vector)(scheme *sc, int len);
     pointer (*mk_foreign_func)(scheme *sc, foreign_func f);
     void (*putstr)(scheme *sc, const char *s);
     void (*putcharacter)(scheme *sc, int c);

     int (*is_string)(pointer p);
     char *(*string_value)(pointer p);
     int (*is_number)(pointer p);
     num (*nvalue)(pointer p);
     long (*ivalue)(pointer p);
     double (*rvalue)(pointer p);
     int (*is_integer)(pointer p);
     int (*is_real)(pointer p);
     int (*is_character)(pointer p);
     long (*charvalue)(pointer p);
     int (*is_list)(scheme *sc, pointer p);
     int (*is_vector)(pointer p);
     int (*list_length)(scheme *sc, pointer vec);
     long (*vector_length)(pointer vec);
     void (*fill_vector)(pointer vec, pointer elem);
     pointer (*vector_elem)(pointer vec, int ielem);
     pointer (*set_vector_elem)(pointer vec, int ielem, pointer newel);
     int (*is_port)(pointer p);

     int (*is_pair)(pointer p);
     pointer (*pair_car)(pointer p);
     pointer (*pair_cdr)(pointer p);
     pointer (*set_car)(pointer p, pointer q);
     pointer (*set_cdr)(pointer p, pointer q);

     int (*is_symbol)(pointer p);
     char *(*symname)(pointer p);

     int (*is_syntax)(pointer p);
     int (*is_proc)(pointer p);
     int (*is_foreign)(pointer p);
     char *(*syntaxname)(pointer p);
     int (*is_closure)(pointer p);
     int (*is_macro)(pointer p);
     pointer (*closure_code)(pointer p);
     pointer (*closure_env)(pointer p);

     int (*is_continuation)(pointer p);
     int (*is_promise)(pointer p);
     int (*is_environment)(pointer p);
     int (*is_immutable)(pointer p);
     void (*setimmutable)(pointer p);
     void (*load_file)(scheme *sc, FILE *fin);
     void (*load_string)(scheme *sc, const char *input);
};
#endif

#if !STANDALONE

typedef struct scheme_registerable{
     foreign_func  f;
     const char* name;
}
scheme_registerable;

void scheme_register_foreign_func_list(scheme * sc,
                                       scheme_registerable * list,
                                       int n);

#endif /* !STANDALONE */

enum scheme_port_kind {
     port_free=0,
     port_file=1,
     port_string=2,
     port_srfi6=4,
     port_input=16,
     port_output=32,
     port_saw_EOF=64
};

typedef struct port {
     unsigned char kind;
     union {
          struct {
               FILE *file;
               int closeit;
#if SHOW_ERROR_LINE
               int curr_line;
               char *filename;
#endif
          } stdio;
          struct {
               char *start;
               char *past_the_end;
               char *curr;
          } string;
     } rep;
} port;

/* cell structure */
struct cell {
     unsigned int _flag;
     union {
          struct {
               char   *_svalue;
               int   _length;
          } _string;
          num _number;
          port *_port;
          foreign_func _ff;
          struct {
               struct cell *_car;
               struct cell *_cdr;
          } _cons;
     } _object;
};

struct scheme {
/* arrays for segments */
     func_alloc malloc;
     func_dealloc free;

/* return code */
     int retcode;
     int tracing;


#define CELL_SEGSIZE    5000  /* # of cells in one segment */
#define CELL_NSEGMENT   10    /* # of segments for cells */
     char *alloc_seg[CELL_NSEGMENT];
     pointer cell_seg[CELL_NSEGMENT];
     int     last_cell_seg;

/* We use 4 registers. */
     pointer args;            /* register for arguments of function */
     pointer envir;           /* stack register for current environment */
     pointer code;            /* register for current code */
     pointer dump;            /* stack register for next evaluation */

     int interactive_repl;    /* are we in an interactive REPL? */

     struct cell _sink;
     pointer sink;            /* when mem. alloc. fails */
     struct cell _NIL;
     pointer NIL;             /* special cell representing empty cell */
     struct cell _HASHT;
     pointer T;               /* special cell representing #t */
     struct cell _HASHF;
     pointer F;               /* special cell representing #f */
     struct cell _EOF_OBJ;
     pointer EOF_OBJ;         /* special cell representing end-of-file object */
     pointer oblist;          /* pointer to symbol table */
     pointer global_env;      /* pointer to global environment */
     pointer c_nest;          /* stack for nested calls from C */

/* global pointers to special symbols */
     pointer LAMBDA;               /* pointer to syntax lambda */
     pointer QUOTE;           /* pointer to syntax quote */

     pointer QQUOTE;               /* pointer to symbol quasiquote */
     pointer UNQUOTE;         /* pointer to symbol unquote */
     pointer UNQUOTESP;       /* pointer to symbol unquote-splicing */
     pointer FEED_TO;         /* => */
     pointer COLON_HOOK;      /* *colon-hook* */
     pointer ERROR_HOOK;      /* *error-hook* */
     pointer SHARP_HOOK;  /* *sharp-hook* */
     pointer COMPILE_HOOK;  /* *compile-hook* */

     pointer free_cell;       /* pointer to top of free cells */
     long    fcells;          /* # of free cells */

     pointer inport;
     pointer outport;
     pointer save_inport;
     pointer loadport;

#define MAXFIL 64
     port load_stack[MAXFIL];     /* Stack of open files for port -1 (LOADing) */
     int nesting_stack[MAXFIL];
     int file_i;
     int nesting;

     char    gc_verbose;      /* if gc_verbose is not zero, print gc status */
     char    no_memory;       /* Whether mem. alloc. has failed */

#define LINESIZE 1024
     char    linebuff[LINESIZE];
#define STRBUFFSIZE 256
     char    strbuff[STRBUFFSIZE];

     FILE *tmpfp;
     int tok;
     int print_flag;
     pointer value;
     int op;

     void *ext_data;     /* For the benefit of foreign functions */
     long gensym_cnt;

     struct scheme_interface *vptr;
     void *dump_base;    /* pointer to base of allocated dump stack */
     int dump_size;      /* number of frames allocated for dump stack */
};

/* operator code */
enum scheme_opcodes {
#define _OP_DEF(A,B,C,D,E,OP) OP,
//#include "opdefines.h"
    _OP_DEF(opexe_0, "load",                           1,  1,       TST_STRING,                      OP_LOAD             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_T0LVL            )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_T1LVL            )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_READ_INTERNAL    )
    _OP_DEF(opexe_0, "gensym",                         0,  0,       0,                               OP_GENSYM           )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_VALUEPRINT       )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_EVAL             )
#if USE_TRACING
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_REAL_EVAL        )
#endif
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_E0ARGS           )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_E1ARGS           )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_APPLY            )
#if USE_TRACING
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_REAL_APPLY       )
    _OP_DEF(opexe_0, "tracing",                        1,  1,       TST_NATURAL,                     OP_TRACING          )
#endif
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_DOMACRO          )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LAMBDA           )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LAMBDA1          )
    _OP_DEF(opexe_0, "make-closure",                   1,  2,       TST_PAIR TST_ENVIRONMENT,        OP_MKCLOSURE        )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_QUOTE            )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_DEF0             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_DEF1             )
    _OP_DEF(opexe_0, "defined?",                       1,  2,       TST_SYMBOL TST_ENVIRONMENT,      OP_DEFP             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_BEGIN            )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_IF0              )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_IF1              )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_SET0             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_SET1             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET0             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET1             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET2             )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET0AST          )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET1AST          )
    _OP_DEF(opexe_0, 0,                                0,  0,       0,                               OP_LET2AST          )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_LET0REC          )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_LET1REC          )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_LET2REC          )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_COND0            )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_COND1            )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_DELAY            )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_AND0             )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_AND1             )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_OR0              )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_OR1              )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_C0STREAM         )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_C1STREAM         )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_MACRO0           )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_MACRO1           )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_CASE0            )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_CASE1            )
    _OP_DEF(opexe_1, 0,                                0,  0,       0,                               OP_CASE2            )
    _OP_DEF(opexe_1, "eval",                           1,  2,       TST_ANY TST_ENVIRONMENT,         OP_PEVAL            )
    _OP_DEF(opexe_1, "apply",                          1,  INF_ARG, TST_NONE,                        OP_PAPPLY           )
    _OP_DEF(opexe_1, "call-with-current-continuation", 1,  1,       TST_NONE,                        OP_CONTINUATION     )
#if USE_MATH
    _OP_DEF(opexe_2, "inexact->exact",                 1,  1,       TST_NUMBER,                      OP_INEX2EX          )
    _OP_DEF(opexe_2, "exp",                            1,  1,       TST_NUMBER,                      OP_EXP              )
    _OP_DEF(opexe_2, "log",                            1,  1,       TST_NUMBER,                      OP_LOG              )
    _OP_DEF(opexe_2, "sin",                            1,  1,       TST_NUMBER,                      OP_SIN              )
    _OP_DEF(opexe_2, "cos",                            1,  1,       TST_NUMBER,                      OP_COS              )
    _OP_DEF(opexe_2, "tan",                            1,  1,       TST_NUMBER,                      OP_TAN              )
    _OP_DEF(opexe_2, "asin",                           1,  1,       TST_NUMBER,                      OP_ASIN             )
    _OP_DEF(opexe_2, "acos",                           1,  1,       TST_NUMBER,                      OP_ACOS             )
    _OP_DEF(opexe_2, "atan",                           1,  2,       TST_NUMBER,                      OP_ATAN             )
    _OP_DEF(opexe_2, "sqrt",                           1,  1,       TST_NUMBER,                      OP_SQRT             )
    _OP_DEF(opexe_2, "expt",                           2,  2,       TST_NUMBER,                      OP_EXPT             )
    _OP_DEF(opexe_2, "floor",                          1,  1,       TST_NUMBER,                      OP_FLOOR            )
    _OP_DEF(opexe_2, "ceiling",                        1,  1,       TST_NUMBER,                      OP_CEILING          )
    _OP_DEF(opexe_2, "truncate",                       1,  1,       TST_NUMBER,                      OP_TRUNCATE         )
    _OP_DEF(opexe_2, "round",                          1,  1,       TST_NUMBER,                      OP_ROUND            )
#endif
    _OP_DEF(opexe_2, "+",                              0,  INF_ARG, TST_NUMBER,                      OP_ADD              )
    _OP_DEF(opexe_2, "-",                              1,  INF_ARG, TST_NUMBER,                      OP_SUB              )
    _OP_DEF(opexe_2, "*",                              0,  INF_ARG, TST_NUMBER,                      OP_MUL              )
    _OP_DEF(opexe_2, "/",                              1,  INF_ARG, TST_NUMBER,                      OP_DIV              )
    _OP_DEF(opexe_2, "quotient",                       1,  INF_ARG, TST_INTEGER,                     OP_INTDIV           )
    _OP_DEF(opexe_2, "remainder",                      2,  2,       TST_INTEGER,                     OP_REM              )
    _OP_DEF(opexe_2, "modulo",                         2,  2,       TST_INTEGER,                     OP_MOD              )
    _OP_DEF(opexe_2, "car",                            1,  1,       TST_PAIR,                        OP_CAR              )
    _OP_DEF(opexe_2, "cdr",                            1,  1,       TST_PAIR,                        OP_CDR              )
    _OP_DEF(opexe_2, "cons",                           2,  2,       TST_NONE,                        OP_CONS             )
    _OP_DEF(opexe_2, "set-car!",                       2,  2,       TST_PAIR TST_ANY,                OP_SETCAR           )
    _OP_DEF(opexe_2, "set-cdr!",                       2,  2,       TST_PAIR TST_ANY,                OP_SETCDR           )
    _OP_DEF(opexe_2, "char->integer",                  1,  1,       TST_CHAR,                        OP_CHAR2INT         )
    _OP_DEF(opexe_2, "integer->char",                  1,  1,       TST_NATURAL,                     OP_INT2CHAR         )
    _OP_DEF(opexe_2, "char-upcase",                    1,  1,       TST_CHAR,                        OP_CHARUPCASE       )
    _OP_DEF(opexe_2, "char-downcase",                  1,  1,       TST_CHAR,                        OP_CHARDNCASE       )
    _OP_DEF(opexe_2, "symbol->string",                 1,  1,       TST_SYMBOL,                      OP_SYM2STR          )
    _OP_DEF(opexe_2, "atom->string",                   1,  2,       TST_ANY TST_NATURAL,             OP_ATOM2STR         )
    _OP_DEF(opexe_2, "string->symbol",                 1,  1,       TST_STRING,                      OP_STR2SYM          )
    _OP_DEF(opexe_2, "string->atom",                   1,  2,       TST_STRING TST_NATURAL,          OP_STR2ATOM         )
    _OP_DEF(opexe_2, "make-string",                    1,  2,       TST_NATURAL TST_CHAR,            OP_MKSTRING         )
    _OP_DEF(opexe_2, "string-length",                  1,  1,       TST_STRING,                      OP_STRLEN           )
    _OP_DEF(opexe_2, "string-ref",                     2,  2,       TST_STRING TST_NATURAL,          OP_STRREF           )
    _OP_DEF(opexe_2, "string-set!",                    3,  3,       TST_STRING TST_NATURAL TST_CHAR, OP_STRSET           )
    _OP_DEF(opexe_2, "string-append",                  0,  INF_ARG, TST_STRING,                      OP_STRAPPEND        )
    _OP_DEF(opexe_2, "substring",                      2,  3,       TST_STRING TST_NATURAL,          OP_SUBSTR           )
    _OP_DEF(opexe_2, "vector",                         0,  INF_ARG, TST_NONE,                        OP_VECTOR           )
    _OP_DEF(opexe_2, "make-vector",                    1,  2,       TST_NATURAL TST_ANY,             OP_MKVECTOR         )
    _OP_DEF(opexe_2, "vector-length",                  1,  1,       TST_VECTOR,                      OP_VECLEN           )
    _OP_DEF(opexe_2, "vector-ref",                     2,  2,       TST_VECTOR TST_NATURAL,          OP_VECREF           )
    _OP_DEF(opexe_2, "vector-set!",                    3,  3,       TST_VECTOR TST_NATURAL TST_ANY,  OP_VECSET           )
    _OP_DEF(opexe_3, "not",                            1,  1,       TST_NONE,                        OP_NOT              )
    _OP_DEF(opexe_3, "boolean?",                       1,  1,       TST_NONE,                        OP_BOOLP            )
    _OP_DEF(opexe_3, "eof-object?",                    1,  1,       TST_NONE,                        OP_EOFOBJP          )
    _OP_DEF(opexe_3, "null?",                          1,  1,       TST_NONE,                        OP_NULLP            )
    _OP_DEF(opexe_3, "=",                              2,  INF_ARG, TST_NUMBER,                      OP_NUMEQ            )
    _OP_DEF(opexe_3, "<",                              2,  INF_ARG, TST_NUMBER,                      OP_LESS             )
    _OP_DEF(opexe_3, ">",                              2,  INF_ARG, TST_NUMBER,                      OP_GRE              )
    _OP_DEF(opexe_3, "<=",                             2,  INF_ARG, TST_NUMBER,                      OP_LEQ              )
    _OP_DEF(opexe_3, ">=",                             2,  INF_ARG, TST_NUMBER,                      OP_GEQ              )
    _OP_DEF(opexe_3, "symbol?",                        1,  1,       TST_ANY,                         OP_SYMBOLP          )
    _OP_DEF(opexe_3, "number?",                        1,  1,       TST_ANY,                         OP_NUMBERP          )
    _OP_DEF(opexe_3, "string?",                        1,  1,       TST_ANY,                         OP_STRINGP          )
    _OP_DEF(opexe_3, "integer?",                       1,  1,       TST_ANY,                         OP_INTEGERP         )
    _OP_DEF(opexe_3, "real?",                          1,  1,       TST_ANY,                         OP_REALP            )
    _OP_DEF(opexe_3, "char?",                          1,  1,       TST_ANY,                         OP_CHARP            )
#if USE_CHAR_CLASSIFIERS
    _OP_DEF(opexe_3, "char-alphabetic?",               1,  1,       TST_CHAR,                        OP_CHARAP           )
    _OP_DEF(opexe_3, "char-numeric?",                  1,  1,       TST_CHAR,                        OP_CHARNP           )
    _OP_DEF(opexe_3, "char-whitespace?",               1,  1,       TST_CHAR,                        OP_CHARWP           )
    _OP_DEF(opexe_3, "char-upper-case?",               1,  1,       TST_CHAR,                        OP_CHARUP           )
    _OP_DEF(opexe_3, "char-lower-case?",               1,  1,       TST_CHAR,                        OP_CHARLP           )
#endif
    _OP_DEF(opexe_3, "port?",                          1,  1,       TST_ANY,                         OP_PORTP            )
    _OP_DEF(opexe_3, "input-port?",                    1,  1,       TST_ANY,                         OP_INPORTP          )
    _OP_DEF(opexe_3, "output-port?",                   1,  1,       TST_ANY,                         OP_OUTPORTP         )
    _OP_DEF(opexe_3, "procedure?",                     1,  1,       TST_ANY,                         OP_PROCP            )
    _OP_DEF(opexe_3, "pair?",                          1,  1,       TST_ANY,                         OP_PAIRP            )
    _OP_DEF(opexe_3, "list?",                          1,  1,       TST_ANY,                         OP_LISTP            )
    _OP_DEF(opexe_3, "environment?",                   1,  1,       TST_ANY,                         OP_ENVP             )
    _OP_DEF(opexe_3, "vector?",                        1,  1,       TST_ANY,                         OP_VECTORP          )
    _OP_DEF(opexe_3, "eq?",                            2,  2,       TST_ANY,                         OP_EQ               )
    _OP_DEF(opexe_3, "eqv?",                           2,  2,       TST_ANY,                         OP_EQV              )
    _OP_DEF(opexe_4, "force",                          1,  1,       TST_ANY,                         OP_FORCE            )
    _OP_DEF(opexe_4, 0,                                0,  0,       0,                               OP_SAVE_FORCED      )
    _OP_DEF(opexe_4, "write",                          1,  2,       TST_ANY TST_OUTPORT,             OP_WRITE            )
    _OP_DEF(opexe_4, "write-char",                     1,  2,       TST_CHAR TST_OUTPORT,            OP_WRITE_CHAR       )
    _OP_DEF(opexe_4, "display",                        1,  2,       TST_ANY TST_OUTPORT,             OP_DISPLAY          )
    _OP_DEF(opexe_4, "newline",                        0,  1,       TST_OUTPORT,                     OP_NEWLINE          )
    _OP_DEF(opexe_4, "error",                          1,  INF_ARG, TST_NONE,                        OP_ERR0             )
    _OP_DEF(opexe_4, 0,                                0,  0,       0,                               OP_ERR1             )
    _OP_DEF(opexe_4, "reverse",                        1,  1,       TST_LIST,                        OP_REVERSE          )
    _OP_DEF(opexe_4, "list*",                          1,  INF_ARG, TST_NONE,                        OP_LIST_STAR        )
    _OP_DEF(opexe_4, "append",                         0,  INF_ARG, TST_NONE,                        OP_APPEND           )
#if USE_PLIST
    _OP_DEF(opexe_4, "put",                            3,  3,       TST_NONE,                        OP_PUT              )
    _OP_DEF(opexe_4, "get",                            2,  2,       TST_NONE,                        OP_GET              )
#endif
    _OP_DEF(opexe_4, "quit",                           0,  1,       TST_NUMBER,                      OP_QUIT             )
    _OP_DEF(opexe_4, "gc",                             0,  0,       0,                               OP_GC               )
    _OP_DEF(opexe_4, "gc-verbose",                     0,  1,       TST_NONE,                        OP_GCVERB           )
    _OP_DEF(opexe_4, "new-segment",                    0,  1,       TST_NUMBER,                      OP_NEWSEGMENT       )
    _OP_DEF(opexe_4, "oblist",                         0,  0,       0,                               OP_OBLIST           )
    _OP_DEF(opexe_4, "current-input-port",             0,  0,       0,                               OP_CURR_INPORT      )
    _OP_DEF(opexe_4, "current-output-port",            0,  0,       0,                               OP_CURR_OUTPORT     )
    _OP_DEF(opexe_4, "open-input-file",                1,  1,       TST_STRING,                      OP_OPEN_INFILE      )
    _OP_DEF(opexe_4, "open-output-file",               1,  1,       TST_STRING,                      OP_OPEN_OUTFILE     )
    _OP_DEF(opexe_4, "open-input-output-file",         1,  1,       TST_STRING,                      OP_OPEN_INOUTFILE   )
#if USE_STRING_PORTS
    _OP_DEF(opexe_4, "open-input-string",              1,  1,       TST_STRING,                      OP_OPEN_INSTRING    )
    _OP_DEF(opexe_4, "open-input-output-string",       1,  1,       TST_STRING,                      OP_OPEN_INOUTSTRING )
    _OP_DEF(opexe_4, "open-output-string",             0,  1,       TST_STRING,                      OP_OPEN_OUTSTRING   )
    _OP_DEF(opexe_4, "get-output-string",              1,  1,       TST_OUTPORT,                     OP_GET_OUTSTRING    )
#endif
    _OP_DEF(opexe_4, "close-input-port",               1,  1,       TST_INPORT,                      OP_CLOSE_INPORT     )
    _OP_DEF(opexe_4, "close-output-port",              1,  1,       TST_OUTPORT,                     OP_CLOSE_OUTPORT    )
    _OP_DEF(opexe_4, "interaction-environment",        0,  0,       0,                               OP_INT_ENV          )
    _OP_DEF(opexe_4, "current-environment",            0,  0,       0,                               OP_CURR_ENV         )
    _OP_DEF(opexe_5, "read",                           0,  1,       TST_INPORT,                      OP_READ             )
    _OP_DEF(opexe_5, "read-char",                      0,  1,       TST_INPORT,                      OP_READ_CHAR        )
    _OP_DEF(opexe_5, "peek-char",                      0,  1,       TST_INPORT,                      OP_PEEK_CHAR        )
    _OP_DEF(opexe_5, "char-ready?",                    0,  1,       TST_INPORT,                      OP_CHAR_READY       )
    _OP_DEF(opexe_5, "set-input-port",                 1,  1,       TST_INPORT,                      OP_SET_INPORT       )
    _OP_DEF(opexe_5, "set-output-port",                1,  1,       TST_OUTPORT,                     OP_SET_OUTPORT      )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDSEXPR          )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDLIST           )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDDOT            )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDQUOTE          )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDQQUOTE         )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDQQUOTEVEC      )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDUNQUOTE        )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDUQTSP          )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_RDVEC            )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_P0LIST           )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_P1LIST           )
    _OP_DEF(opexe_5, 0,                                0,  0,       0,                               OP_PVECFROM         )
    _OP_DEF(opexe_6, "length",                         1,  1,       TST_LIST,                        OP_LIST_LENGTH      )
    _OP_DEF(opexe_6, "assq",                           2,  2,       TST_NONE,                        OP_ASSQ             )
    _OP_DEF(opexe_6, "get-closure-code",               1,  1,       TST_NONE,                        OP_GET_CLOSURE      )
    _OP_DEF(opexe_6, "closure?",                       1,  1,       TST_NONE,                        OP_CLOSUREP         )
    _OP_DEF(opexe_6, "macro?",                         1,  1,       TST_NONE,                        OP_MACROP           )
#undef _OP_DEF

     OP_MAXDEFINED
};


#define cons(sc,a,b) _cons(sc,a,b,0)
#define immutable_cons(sc,a,b) _cons(sc,a,b,1)

int is_string(pointer p);
char *string_value(pointer p);
int is_number(pointer p);
num nvalue(pointer p);
long ivalue(pointer p);
double rvalue(pointer p);
int is_integer(pointer p);
int is_real(pointer p);
int is_character(pointer p);
long charvalue(pointer p);
int is_vector(pointer p);

int is_port(pointer p);

int is_pair(pointer p);
pointer pair_car(pointer p);
pointer pair_cdr(pointer p);
pointer set_car(pointer p, pointer q);
pointer set_cdr(pointer p, pointer q);

int is_symbol(pointer p);
char *symname(pointer p);
int hasprop(pointer p);

int is_syntax(pointer p);
int is_proc(pointer p);
int is_foreign(pointer p);
char *syntaxname(pointer p);
int is_closure(pointer p);
#ifdef USE_MACRO
int is_macro(pointer p);
#endif
pointer closure_code(pointer p);
pointer closure_env(pointer p);

int is_continuation(pointer p);
int is_promise(pointer p);
int is_environment(pointer p);
int is_immutable(pointer p);
void setimmutable(pointer p);

#ifdef __cplusplus
}
#endif

#endif


/*
Local variables:
c-file-style: "k&r"
End:
*/

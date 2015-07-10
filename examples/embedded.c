
#include "scheme.h"

#include <stdio.h>
#include <stdlib.h>

pointer print(scheme* scm, pointer args){
    if(args == scm->NIL) return scm->NIL;

    if(scm->vptr->is_string(scm->vptr->pair_car(args))){
        printf("%s\n", scm->vptr->string_value(scm->vptr->pair_car(args)));
    }
}

pointer print_hello(scheme* scm, pointer args){
    printf("HELLO\n");
}

pointer square(scheme* scm, pointer args) {
    if(args == scm->NIL) return scm->NIL;

    if(scm->vptr->is_number(scm->vptr->pair_car(args))){
        int val = scm->vptr->ivalue(scm->vptr->pair_car(args));
        return scm->vptr->mk_integer(scm, val*val);
    }
}

int main(int argc, char** argv){
    scheme* scm = NULL;

    scm = scheme_init_new();

    /* Load init.scm */
    FILE *finit = fopen("init.scm", "r");
    scheme_load_file(scm, finit);
    fclose(finit);

    scheme_define(scm,
                  scm->global_env,
                  scheme_make_symbol(scm, "print_hello"),
                  scheme_make_foreign_func(scm, print_hello));

    scheme_define(scm,
                  scm->global_env,
                  scheme_make_symbol(scm, "square"),
                  scheme_make_foreign_func(scm, square));

    scheme_define(scm,
                  scm->global_env,
                  scheme_make_symbol(scm, "print"),
                  scheme_make_foreign_func(scm, print));

    /* Run first example */
    char *hello_scm = "(print_hello)";
    scheme_load_string(scm, hello_scm);

    /* Run second example */
    char *square_scm = "(print (string-append \"Square: \" (number->string (square 4)) ))";
    scheme_load_string(scm, square_scm);

    scheme_deinit(scm);

    return 0;
}

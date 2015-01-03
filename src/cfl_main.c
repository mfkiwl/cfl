#include "cfl_parser.h"
#include "cfl_typechecker.h"
#include "cfl_eval.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    cfl_node node;

    if(!cfl_parse_file(&node, "lol.cfl"))
        return 1;

    cfl_print_node(&node);

    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$"
           "\n          TYPE            "
           "\n$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n");

    cfl_type* type = cfl_typecheck(&node);

    if(!type)
    {
        cfl_delete_node(&node);

        printf("ERROR: Failed to typecheck\n");
    }

    cfl_print_type(type);

    cfl_delete_type(type);
    free(type);

    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>"
           "\n         RESULT           "
           "\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");

    if(!cfl_evaluate(&node))
    {
        cfl_delete_node(&node);

        return 1;
    }

    cfl_print_node(&node);

    cfl_delete_node(&node);

    return 0;
}

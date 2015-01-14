#include "cfl_type.h"

#include <stdio.h>

bool cfl_type_error;

void* cfl_type_malloc(size_t size)
{
    void* result = malloc(size);

    if(!result)
    {   
        fprintf(stderr, "MEMORY ERROR: Ran out of memory "
                        "while typechecking");

        cfl_type_error = true;
    }   

    return result;
}

void cfl_reset_type_error_flag(void)
{
    cfl_type_error = false;
}

bool cfl_get_type_error_flag(void)
{
    return cfl_type_error;
}

void cfl_type_error_failure(void)
{
    fprintf(stderr, "TYPE ERROR: Could not type expression\n");

    cfl_type_error = true;
}
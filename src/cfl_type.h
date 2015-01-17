#ifndef _CFL_TYPE_H_
#define _CFL_TYPE_H_

#include "cfl_ast.h"
#include "cfl_program.h"
#include "cfl_type.types.h"
#include "cfl_type.error.h"
#include "cfl_type.equation.h"
#include "cfl_type.generate.h"

void cfl_create_type_variable(cfl_type* node, unsigned int id);
void cfl_create_type_bool(cfl_type* node);
void cfl_create_type_integer(cfl_type* node);
void cfl_create_type_char(cfl_type* node);
void cfl_create_type_list(cfl_type* node, cfl_type* content);
void cfl_create_type_tuple(cfl_type* node,
                           unsigned int number_of_children,
                           cfl_type** children);
void cfl_create_type_arrow(cfl_type* node, cfl_type* input, cfl_type* output);

int cfl_compare_type(cfl_type* left, cfl_type* right);

int cfl_copy_type(cfl_type* target, cfl_type* node);

void cfl_delete_type(cfl_type* node);
void cfl_free_type(cfl_type* node);

void cfl_print_type(cfl_type* node);

void cfl_delete_type_equation_chain(cfl_type_equation_chain* chain);

int cfl_add_equation(cfl_type_equation_chain* head, cfl_type* left, cfl_type* right);
int cfl_add_equation_from_copies(cfl_type_equation_chain* head,
                                 cfl_type* left,
                                 cfl_type* right);

int cfl_close_type_equation_chain(cfl_type_equation_chain* head);
int cfl_ensure_type_equation_chain_consistency(cfl_type_equation_chain* chain);

cfl_type* cfl_substitute_type(cfl_type_equation_chain* head, cfl_type* node);

bool cfl_typecheck(cfl_program* program);

#endif

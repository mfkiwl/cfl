#ifndef _CFL_TYPE_GENERATE_H_
#define _CFL_TYPE_GENERATE_H_

#include "cfl_type.types.h"

unsigned int cfl_lookup_hypothesis(cfl_type_hypothesis_chain* chain, char* name);

void cfl_remove_n_hypotheses(cfl_type_hypothesis_chain* hypothesis_head, unsigned int n);

cfl_type* cfl_generate_type_equation_chain(cfl_type_equation_chain* equation_head,
                                           cfl_type_hypothesis_chain* hypothesis_head,
                                           cfl_node* node);

unsigned int cfl_generate_global_hypotheses(
        cfl_type_equation_chain* equation_head,
        cfl_type_hypothesis_chain* hypothesis_head);

unsigned int cfl_setup_definitions(cfl_type_equation_chain* equation_head,
                                   cfl_type_hypothesis_chain* hypothesis_head,
                                   cfl_definition_list* definitions);

#endif

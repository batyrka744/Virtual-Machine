#pragma once
#include "../my_ass.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Stack_canary.h"


struct processor;

typedef void (*command_func_t)(struct processor* v_m, size_t* breakflag, errors* err);

#define check_valid(v_m, breakflag, err)\
    my_assert(v_m != NULL,);\
    my_assert(breakflag != NULL,);\
    my_assert(err != NULL,);

void exec_hlt    (processor* v_m, size_t* breakflag, errors* err);

void exec_push   (processor* v_m, size_t* breakflag, errors* err);

void exec_add    (processor* v_m, size_t* breakflag, errors* err);

void exec_sub    (processor* v_m, size_t* breakflag, errors* err);

void exec_div    (processor* v_m, size_t* breakflag, errors* err);

void exec_out    (processor* v_m, size_t* breakflag, errors* err);

void exec_pop    (processor* v_m, size_t* breakflag, errors* err);

void exec_push_r (processor* v_m, size_t* breakflag, errors* err);

void exec_mul    (processor* v_m, size_t* breakflag, errors* err);

void exec_jmp    (processor* v_m, size_t* breakflag, errors* err);

void exec_ja     (processor* v_m, size_t* breakflag, errors* err);

void exec_jae    (processor* v_m, size_t* breakflag, errors* err);

void exec_jb     (processor* v_m, size_t* breakflag, errors* err);

void exec_jbe    (processor* v_m, size_t* breakflag, errors* err);

void exec_je     (processor* v_m, size_t* breakflag, errors* err);

void exec_jne    (processor* v_m, size_t* breakflag, errors* err);

void exec_call   (processor* v_m, size_t* breakflag, errors* err);

void exec_ret    (processor* v_m, size_t* breakflag, errors* err);

void exec_in     (processor* v_m, size_t* breakflag, errors* err);

void exec_sqrt   (processor* v_m, size_t* breakflag, errors* err);

void exec_push_m (processor* v_m, size_t* breakflag, errors* err);

void exec_pop_m  (processor* v_m, size_t* breakflag, errors* err);

void exec_dmp    (processor* v_m, size_t* breakflag, errors* err);

void exec_push_mr(processor* v_m, size_t* breakflag, errors* err);

void exec_pop_mr (processor* v_m, size_t* breakflag, errors* err);

void check_memory(processor* v_m, size_t mem_addr);
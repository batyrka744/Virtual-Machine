#include "process.h"


void exec_hlt(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err)
    (*breakflag)++;
    v_m->pc += 1;
}

void exec_push(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackPush(&v_m->stk, v_m->code[v_m->pc + 1]);
    v_m->pc += 2;
}

void exec_add(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a + b);
    v_m->pc += 1;
}

void exec_sub(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a - b);
    v_m->pc += 1;
}

void exec_mul(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a * b);
    v_m->pc += 1;
}

void exec_div(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    my_assert(b != 0,);

    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a / b);
    v_m->pc += 1;
}

void exec_sqrt(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    my_assert(b >= 0,);
    StackPush(&v_m->stk, sqrt(b));
    v_m->pc += 1;
}

void exec_push_r(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackPush(&v_m->stk, v_m->registers[(int)v_m->code[v_m->pc + 1]]);
    v_m->pc += 2;
}

void exec_pop(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    v_m->registers[(int)v_m->code[v_m->pc + 1]] = StackPop(&v_m->stk, err);
    v_m->pc += 2;
}

void exec_out(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    printf(GREEN "%lg\n" RESET, StackPop(&v_m->stk, err));
    v_m->pc += 1;
}

void exec_in(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t x = 0;

    printf(MAGENTA "Enter a value\n" RESET);
    fflush(stdout);

    int good = scanf("%lg", &x);
    my_assert(good == 1,)

    StackPush(&v_m->stk, x);
    v_m->pc += 1;
}

void exec_call(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackPush(&v_m->ret_stk, v_m->pc + 2);
    v_m->pc = (int)v_m->code[v_m->pc + 1];
}

void exec_ret(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    v_m->pc = (int)StackPop(&v_m->ret_stk, err);
}

void exec_jmp(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    v_m->pc = (int)v_m->code[v_m->pc + 1];
}

void exec_ja(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b > a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jae(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b >= a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jb(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b < a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jbe(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b <= a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_je(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b == a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jne(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b != a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_push_m(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    size_t mem_addr = (size_t)v_m->code[v_m->pc + 1];
    StackElem_t b = v_m->memory[mem_addr];
    StackPush(&v_m->stk, b);
    v_m->pc += 2;
}

void exec_pop_m(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    size_t mem_addr = (size_t)v_m->code[v_m->pc + 1];
    check_memory(v_m, mem_addr);
    v_m->memory[mem_addr] = StackPop(&v_m->stk, err);
    v_m->pc += 2;
}

void exec_dmp(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    fprintf(DUMP_MEMORY_FILE, "\n");
    for (size_t i = 0; i < DIAMETR; i++) {
        for (size_t j = 0; j < DIAMETR; j++) {

            size_t x = DIAMETR * i + j;
            char pixel = ' ';
            if (x < v_m->memory_size && v_m->memory[x] != 0) {
                pixel = '*';
            } 
            fprintf(DUMP_MEMORY_FILE, "%c ", pixel);
        } 
        fprintf(DUMP_MEMORY_FILE, "\n");
    }
    v_m->pc += 1;
}

void exec_push_mr(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    size_t reg_idx = (size_t)v_m->code[v_m->pc + 1];
    size_t mem_addr = (size_t)v_m->registers[reg_idx];

    check_memory(v_m, mem_addr);

    StackPush(&v_m->stk, v_m->memory[mem_addr]);
    v_m->pc += 2;
}

void exec_pop_mr(processor* v_m, size_t* breakflag, errors* err) {
    check_valid(v_m, breakflag, err);
    
    size_t reg_idx = (size_t)v_m->code[v_m->pc + 1];
    size_t mem_addr = (size_t)v_m->registers[reg_idx];

    check_memory(v_m, mem_addr);

    v_m->memory[mem_addr] = StackPop(&v_m->stk, err);
    v_m->pc += 2;
}

void check_memory(processor* v_m, size_t mem_addr) {
    my_assert(v_m != NULL,);
    if (mem_addr >= MAX_MEMORY_SIZE) {
        printf(YELLOW "Change \"MAX_MEMORY_SIZE\" if you want more memory\n" RESET);
        return;

    } 
    
    if (mem_addr >= v_m->memory_size) {
        size_t old_size = v_m->memory_size;
        size_t new_size = (mem_addr + 1) * 2;

        StackElem_t* new_memory = (StackElem_t*)realloc(v_m->memory, new_size * sizeof(StackElem_t));
        my_assert(new_memory != NULL,);

        memset(new_memory + old_size, 0, (new_size - old_size) * sizeof(StackElem_t));
        v_m->memory = new_memory;
        v_m->memory_size = new_size;
    }
}
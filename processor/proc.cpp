#include "process.h"


int main() {

    clean_file();  

    processor intel;
    InitIntel(&intel);
    
    intel.code = lf_buffer(after_ass, &intel.buffer_size);  
    my_assert(intel.code != NULL, -1);  

    ReadFromBuffer(&intel);

    DestroyIntel(&intel);
    return 0;
}

/**
 * @brief reads elements from buffer and parses them
 * 
 * @param[in, out] v_m pointer on structure with processor's characteristics
 */
void ReadFromBuffer(processor* v_m) {
    my_assert(v_m != NULL,);  

    int command = 0;

    errors err = ERR_OK;
    size_t break_flag = 0;

    while ((v_m->pc < v_m->buffer_size) && (break_flag == 0)) {   

        CodeDump(v_m);
        command = (int)v_m->code[v_m->pc];
        
        execution(v_m, command, &break_flag, &err);
        my_assert(err == ERR_OK,);
    }
}
    
/**
 * @brief completes commands
 * 
 * @param[in, out] v_m pointer to structure with characteristic of processor
 * @param[in] command number of command
 * @param[out] break_flag 1 if it needs to stop reading else 0
 * @param[out] err ERR_OK if all is OK else error
 */
void execution(processor* v_m, int command, size_t* break_flag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(break_flag != NULL,);
    my_assert(err != NULL,);
    my_assert(command >= 0,);
    my_assert(command <= sizeof(command_table) / sizeof(command_table[0]),);
    
    command_table[command].func(v_m, break_flag, err);
}

void exec_hlt(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);

    (*breakflag)++;
    v_m->pc += 1;
}

void exec_push(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackPush(&v_m->stk, v_m->code[v_m->pc + 1]);
    v_m->pc += 2;
}

void exec_add(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a + b);
    v_m->pc += 1;
}

void exec_sub(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a - b);
    v_m->pc += 1;
}

void exec_mul(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a * b);
    v_m->pc += 1;
}

void exec_div(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    my_assert(b != 0,);

    StackElem_t a = StackPop(&v_m->stk, err);
    StackPush(&v_m->stk, a / b);
    v_m->pc += 1;
}

void exec_sqrt(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    my_assert(b >= 0,);
    StackPush(&v_m->stk, sqrt(b));
    v_m->pc += 1;
}

void exec_push_r(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackPush(&v_m->stk, v_m->registers[(int)v_m->code[v_m->pc + 1]]);
    v_m->pc += 2;
}

void exec_pop(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    v_m->registers[(int)v_m->code[v_m->pc + 1]] = StackPop(&v_m->stk, err);
    v_m->pc += 2;
}

void exec_out(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    printf(GREEN "%lg\n" RESET, StackPop(&v_m->stk, err));
    v_m->pc += 1;
}

void exec_in(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t x = 0;

    printf(MAGENTA "Enter a value\n" RESET);
    fflush(stdout);
    my_assert(scanf("%lg", &x) == 1,)

    StackPush(&v_m->stk, x);
    v_m->pc += 1;
}

void exec_call(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackPush(&v_m->ret_stk, v_m->pc + 2);
    v_m->pc = (int)v_m->code[v_m->pc + 1];
}

void exec_ret(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    v_m->pc = (int)StackPop(&v_m->ret_stk, err);
}

void exec_jmp(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    v_m->pc = (int)v_m->code[v_m->pc + 1];
}

void exec_ja(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b > a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jae(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b >= a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jb(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b < a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jbe(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b <= a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_je(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b == a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_jne(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    StackElem_t b = StackPop(&v_m->stk, err);
    StackElem_t a = StackPop(&v_m->stk, err);

    if (b != a) 
        v_m->pc = (int)v_m->code[v_m->pc + 1];
    else
        v_m->pc += 2;
}

void exec_push_m(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    size_t mem_addr = (size_t)v_m->code[v_m->pc + 1];
    StackElem_t b = v_m->memory[mem_addr];
    StackPush(&v_m->stk, b);
    v_m->pc += 2;
}

void exec_pop_m(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    size_t mem_addr = (size_t)v_m->code[v_m->pc + 1];
    check_memory(v_m, mem_addr);
    v_m->memory[mem_addr] = StackPop(&v_m->stk, err);
    v_m->pc += 2;
}

void exec_dmp(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
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
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
    size_t reg_idx = (size_t)v_m->code[v_m->pc + 1];
    size_t mem_addr = (size_t)v_m->registers[reg_idx];

    check_memory(v_m, mem_addr);

    StackPush(&v_m->stk, v_m->memory[mem_addr]);
    v_m->pc += 2;
}

void exec_pop_mr(processor* v_m, size_t* breakflag, errors* err) {
    my_assert(v_m != NULL,);
    my_assert(breakflag != NULL,);
    my_assert(err != NULL,);
    
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

    } else if (mem_addr >= v_m->memory_size) {
        size_t old_size = v_m->memory_size;
        size_t new_size = (mem_addr + 1) * 2;

        StackElem_t* new_memory = (StackElem_t*)realloc(v_m->memory, new_size * sizeof(StackElem_t));
        my_assert(new_memory != NULL,);

        memset(new_memory + old_size, 0, (new_size - old_size) * sizeof(StackElem_t));
        v_m->memory = new_memory;
        v_m->memory_size = new_size;
    }
}

/**
 * @brief prints into the file buffer's dump
 * 
 * @param[in, out] v_m pointer to structure with characteristic of processor 
 */
void CodeDump(processor* v_m) {
    my_assert(v_m != NULL,);
    
    FILE* file = fopen(LOG_FILE, "a");
    my_assert(file != NULL,);

    size_t empty_flag = 0;
    size_t arg_flag = 0;

    fprintf(file, "========= BYTE-CODE =========\n");
    for (size_t i = 0; i < v_m->buffer_size && empty_flag < 10; i++) {

        fprintf(file, "[%zu] %lg", i, v_m->code[i]);

        if (arg_flag == 1) {
            arg_flag = 0;
        } else {
            const char* name = CommandName((int)v_m->code[i], &arg_flag);
            if (*name != '\0') 
                fprintf(file, "   (%s)", name);
        }
        
        if (i == v_m->pc) 
            fprintf(file, "      [<--------");
        
        fprintf(file, "\n");

        if (v_m->code[i] == 0) empty_flag++;
    }

    fprintf(file, "\n");
    fclose(file);
}

/**
 * @brief cleans txt file
 * 
 */
void clean_file(void) {
    fclose(fopen(LOG_FILE, "w"));
}


/**
 * @brief reads file and puts text in the buffer
 * 
 * @param[in] name name of file with text
 * @param[out] out_size the real number of bytes
 * @return char* that need to free -- dynamic memory
 */
char* ReadFromFile(const char* name, size_t* out_size) {

    my_assert(out_size != NULL, NULL);
    my_assert(name != NULL, NULL);

    FILE* file = fopen(name, "rb");
    my_assert(file != NULL, NULL);

    struct stat file_info = {0};
    int fd = fileno(file);
    my_assert(fstat(fd, &file_info) == 0, NULL);
    size_t file_size = (size_t)file_info.st_size;


    char* buffer = (char*)calloc(file_size + 5, sizeof(char));
    my_assert(buffer != NULL, NULL);

    *out_size = fread(buffer, sizeof(char), file_size , file);
    buffer[*out_size] = '\0';
     

    fclose(file);

    return buffer;
}

/**
 * @brief returns buffer with
 * 
 * @param[in] name name of file with byte-code
 * @param[out] buffer_size size of buffer with code
 * @return StackElem_t* pointer to buffer
 */
StackElem_t* lf_buffer(const char* name, size_t* buffer_size) {
    my_assert(name != NULL, NULL);
    my_assert(buffer_size != NULL, NULL);

    size_t outsize = 0;
    char* buffer = ReadFromFile(name, &outsize);
    my_assert(buffer != NULL, NULL);

    *buffer_size = outsize / sizeof(StackElem_t);

    return (StackElem_t*)buffer;
}

/**
 * @brief returns string with name of file 
 * 
 * @param[in] command command's opcode
 * @param[out] arg_flag returns 1 if command has argument else 0
 * @return const char* name of command
 */
const char* CommandName(int command, size_t* arg_flag) {
    my_assert(arg_flag != NULL, "Error");
    my_assert(command >= 0, "Error");
    my_assert(command <= sizeof(command_table) / sizeof(command_table[0]), "Error")

    *arg_flag = (command_table[command].has_arg != NO) ? 1 : 0;
    return command_table[command].name;
}



/**
 * @brief cleans memory and finishes work
 * 
 * @param[in, out] intel structure with processor's characteristic
 */
void DestroyIntel(processor* intel) {
    my_assert(intel != NULL,);

    StackDestroy(&intel->stk);
    StackDestroy(&intel->ret_stk);
    intel->buffer_size = POISON_SIZE;
    intel->pc = POISON_SIZE;
    
    free(intel->memory);
    free(intel->code);
}

/**
 * @brief starts work and initializate structure
 * 
 * @param[in, out] v_m structure with processor's characteristics 
 */
void InitIntel(processor* v_m) {
    STACK_INIT(v_m->stk, STACK_CAPACITY);   
    STACK_INIT(v_m->ret_stk, RECURSION_DEEPTH);    
    v_m->buffer_size = 0;
    v_m->pc = 0;

    v_m->memory = (StackElem_t*)calloc(DIAMETR * DIAMETR, sizeof(StackElem_t));
    my_assert(v_m->memory != NULL,);
    v_m->memory_size = DIAMETR * DIAMETR;
}

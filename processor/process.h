#include "../hnik.h"
#include <sys/types.h>
#include <sys/stat.h>

//----------------------------------------------- CONSTS -----------------------------------------------
#define COMMAND_LENGTH 64
#define LOG_FILE "log.txt"
#define STACK_CAPACITY 10
#define RECURSION_DEEPTH 10
#define MAX_MEMORY_SIZE 5000

#define DIAMETR 5
#define DUMP_MEMORY_FILE stdout

#define YES 1
#define NO 0


const char* before_ass = "../assembler/input1.txt";
const char* after_ass = "input2.txt";

// 5 //
// правки все внесешь
// квадратка и факториал точно на 7 + красивый свитч
// оперативка на // 8 // 9 // 


#define STACK_DEBUG

//-------------------------------------------- STRUCTURES -----------------------------------------------

struct processor {
    stack_t stk;
    stack_t ret_stk;

    StackElem_t* code;
    size_t buffer_size;
    int pc; 

    StackElem_t* memory;
    size_t memory_size;

    StackElem_t registers[NUM_REGIST] = {0}; 
};

typedef void (*command_func_t)(struct processor* v_m, size_t* breakflag, errors* err);

struct CommandHandler {
    const char* name;
    commands opcode;
    command_func_t func;
    int has_arg;
};


//--------------------------------------------- FUNCTION DECLARATIONS ---------------------------------

char* ReadFromFile(const char* name, size_t* out_size);

StackElem_t* lf_buffer(const char* name, size_t* buffer_size);

void ReadFromBuffer(processor* v_m);

void CodeDump(processor* v_m);

void clean_file(void);

const char* CommandName(int command, size_t* arg_flag);

void execution(processor* v_m, int command, size_t* break_flag, errors* err);

void DestroyIntel(processor* intel);

void InitIntel(processor* v_m);

//----------------------------------------- FUNCTIONS FOR ARRAY WITH STRUCTURES -------------------------------------------

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

//------------------------------------------ ARRAY WITH COMMANDS ----------------------------------------------
static const CommandHandler command_table[] = {
    {"HLT",  HLT,  exec_hlt, NO}, 
    {"PUSH", PUSH, exec_push, YES},
    {"ADD",  ADD,  exec_add, NO},
    {"SUB",  SUB,  exec_sub, NO},
    {"DIV",  DIV,  exec_div, NO},
    {"OUT",  OUT,  exec_out, NO},
    {"POP",  POP,  exec_pop, YES},
    {"PUSHr", PUSHr, exec_push_r, YES},
    {"MUL",  MUL,  exec_mul, NO},

    {"JMP",  JMP,  exec_jmp, YES},
    {"JA",   JA,   exec_ja, YES},
    {"JAE",  JAE,  exec_jae, YES},
    {"JB",   JB,   exec_jb, YES},
    {"JBE",  JBE,  exec_jbe, YES},
    {"JE",   JE,   exec_je, YES},
    {"JNE",  JNE,  exec_jne, YES},

    {"CALL", CALL, exec_call, YES},
    {"RET",  RET,  exec_ret, NO},  

    {"IN",   IN,   exec_in, NO},
    {"SQRT", SQRT, exec_sqrt, NO},
    {"PUSHm", PUSHm, exec_push_m, YES},
    {"POPm", POPm, exec_pop_m, YES},
    {"DMP", DMP, exec_dmp, NO},
    {"PUSHmr", PUSHmr, exec_push_mr, YES},
    {"POPmr", POPmr, exec_pop_mr, YES},
};
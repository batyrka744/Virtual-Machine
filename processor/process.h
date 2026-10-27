#include "../hnik.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define COMMAND_LENGTH 64
#define LOG_FILE "log.txt"
#define STACK_CAPACITY 10
#define RECURSION_DEEPTH 10

const char* before_ass = "../assembler/input1.txt";
const char* after_ass = "input2.txt";


#define STACK_DEBUG


#define my_assert(exp, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s, file %s, line %d]\n" RESET, __FILE__, __LINE__, #exp,  __FILE__, __LINE__);\
    return ret;} 


struct proccessor
{
    stack_t stk;
    stack_t ret_stk;

    StackElem_t* code;
    size_t buffer_size;
    int pc; 

    StackElem_t registers[NUM_REGIST] = {0}; 
};


Line* SplitLines(char* buffer, size_t file_size, size_t* nLines);

char* ReadFromFile(const char* name, size_t* out_size);

StackElem_t* lf_buffer(const char* name, size_t* buffer_size);

void ReadFromBuffer(proccessor* v_m);

void CodeDump(proccessor* v_m);

void clean_file(void);

const char* CommandName(int command, size_t* arg_flag);

//void Assembler(FILE* file_output, Line* index, char* buffer, size_t nLines);

void execution(proccessor* v_m, int command, size_t* break_flag, errors* err);

void DestroyIntel(proccessor* intel);
#include "assem.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define COMMAND_LENGTH 64
#define STACK_DEBUG

#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

#define STACK_DEBUG
#define LOG_FILE "log.txt"

#define my_assert(exp, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s, file %s, line %d]\n" RESET, __FILE__, __LINE__, #exp,  __FILE__, __LINE__);\
    return ret;} 


typedef double ass_t;

struct proccessor
{
    stack_t stk;
    double* code;
    size_t buffer_size;
    int pc; 
    double registers[NUM_REGIST] = {0}; 
};


Line* SplitLines(char* buffer, size_t file_size, size_t* nLines);

char* ReadFromFile(const char* name, size_t* out_size);

double* lf_buffer(const char* name, size_t* buffer_size);

void ReadFromBuffer(proccessor* v_m);

void CodeDump(proccessor* v_m);

void clean_file(void);

const char* CommandName(int command, size_t* arg_flag);

void Assembler(FILE* file_output, Line* index, char* buffer, size_t nLines);

void execution(proccessor* v_m, int command, size_t* break_flag, errors* err);
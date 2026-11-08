#include "Stack_canary.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

//--------------------------------------------- MACROSES -------------------------------------

#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

#define my_assert(exp, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s]\n" RESET, __FILE__, __LINE__, #exp);\
    return ret;}     //fprintf

#define my_assert_tell(exp, text, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s]\n" RESET, __FILE__, __LINE__, text);\
    return ret;} 

//---------------------------------------------- ENUMS ---------------------------------------------------

enum commands {
    HLT = 0,
    PUSH = 1,
    ADD = 2, 
    SUB = 3, 
    DIV = 4, 
    OUT = 5, 
    POP = 6, 
    PUSHr = 7, 
    MUL = 8, 

    JMP = 9, 
    JA = 10,    // <
    JAE = 11,   // <=
    JB = 12,    // >
    JBE = 13,   // >=  
    JE = 14,    // ==
    JNE = 15,    // !=

    CALL = 16,
    RET = 17,

    IN = 18,
    SQRT = 19,

    PUSHm = 20,
    POPm = 21,
    DMP = 22,
    PUSHmr = 23,
    POPmr = 24
};

//------------------------------------------------- REGISTERS ------------------------------------------------------
#define NUM_REGIST 8

enum reg_nums {
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

struct Registr {
    const char* name;
    int num;
    size_t len;
};

static Registr registers[NUM_REGIST] = {
    {"AX", AX, strlen("AX")}, 
    {"BX", BX, strlen("BX")},
    {"CX", CX, strlen("CX")},
    {"DX", DX, strlen("DX")},
};

//----------------------------------------------------- FOR READ -------------------------------------------------------
struct Line {char* str; size_t len;};
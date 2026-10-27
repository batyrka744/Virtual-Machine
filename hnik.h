#include "Stack_canary.h"

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
    printf(YELLOW "%s:%d: [Assertation failed: %s, file %s, line %d]\n" RESET, __FILE__, __LINE__, #exp,  __FILE__, __LINE__);\
    return ret;} 

#define NUM_REGIST 8


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
    IN = 18 
};

//------------------------------------------------- REGISTERS ------------------------------------------------------
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
    {.name = "AX", .num = AX, .len = 2}, 
    {.name = "BX", .num = BX, .len = 2},
    {.name = "CX", .num = CX, .len = 2},
    {.name = "DX", .num = DX, .len = 2},
};

struct Line {char* str; size_t len;};
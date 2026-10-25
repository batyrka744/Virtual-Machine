#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

#include "processor/Stack_canary.h"
//--------------------------------------------- MACROSES -------------------------------------------

#define COMMAND_LENGTH 64
#define NUM_REGIST 8
#define MAX_LABELS 67
#define NOT_FOUNDED -1


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

//--------------------------------------------- CONSTS -------------------------------------------------

const char* before_ass = "input1.txt";
const char* after_ass = "input2.txt";

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

enum reg_nums {
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};



//------------------------------------------------ LABELS -----------------------------------------------------------

struct Label {
    char name[COMMAND_LENGTH];
    int address;
};

struct LabelsCase {   //в общую структуру
    Label labels[MAX_LABELS];
    size_t count;
};

//-------------------------------------------------- FOR READ --------------------------------------------------------------


struct Line {char* str; size_t len;};

struct project {
    FILE* file_write;
    size_t out_size, nLines;
    char* buffer;
    Line* index;
};

enum ArgType {
    ARG_NO = 0,
    REG = 1,
    ARG_VALUE = 2,
    LABEL = 3,
};

struct CommandInfo {
    const char* name;
    commands opcode;
    ArgType arg;
};

static CommandInfo all_commands[] = {
    {.name = "HLT",  .opcode = HLT,  .arg = ARG_NO}, 
    {.name = "PUSH", .opcode = PUSH, .arg = ARG_VALUE},
    {.name = "ADD",  .opcode = ADD,  .arg = ARG_NO},
    {.name = "SUB",  .opcode = SUB,  .arg = ARG_NO},
    {.name = "DIV",  .opcode = DIV,  .arg = ARG_NO},
    {.name = "OUT",  .opcode = OUT,  .arg = ARG_NO},
    {.name = "POP",  .opcode = POP,  .arg = REG},
    {.name = "MUL",  .opcode = MUL,  .arg = ARG_NO},
    {.name = "JMP",  .opcode = JMP,  .arg = LABEL},
    {.name = "JA",   .opcode = JA,   .arg = LABEL},
    {.name = "JAE",  .opcode = JAE,  .arg = LABEL},
    {.name = "JB",   .opcode = JB,   .arg = LABEL},
    {.name = "JBE",  .opcode = JBE,  .arg = LABEL},
    {.name = "JE",   .opcode = JE,   .arg = LABEL},
    {.name = "JNE",  .opcode = JNE,  .arg = LABEL},
    {.name = "CALL", .opcode = CALL, .arg = LABEL},
    {.name = "RET",  .opcode = RET,  .arg = ARG_NO},  
    {.name = "IN",   .opcode = IN,   .arg = ARG_NO},
};

//-------------------------------------------------- ASS -----------------------------------------------------------

struct AsmState {

    StackElem_t* bytecode;
    size_t ip;
    size_t capacity;
    int pass;
    LabelsCase Case;
    int need_second_pass;
};

//------------------------------------------------- REGISTERS ------------------------------------------------------

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

//---------------------------------------------- FUNCTIONS DECLARATION --------------------------------------------

char* ReadFromFile(const char* name, size_t* out_size);

Line* SplitLines(char* buffer, size_t file_size, size_t* nLines);

void Assembler(project* ass_main, AsmState* state);

void add_label(LabelsCase* Case, const char* name, size_t address);

int find_label(const LabelsCase* Case, const char* name);

int ParseLine(const char* line, AsmState* state);

int ParseNoArg(const CommandInfo* cmd, const char* end_of_line, AsmState* state);

int ParseWithArg(const CommandInfo* cmd, const char* end_of_line, AsmState* state);

int CheckRubbish(const char* str);

static int FindRegister(const char* reg_name);

CommandInfo* FindCommand(const char* name);

void start_work(project* ass_main, AsmState* state);

void finish_work(AsmState* state, project* ass_main);
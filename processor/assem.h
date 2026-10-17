#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

#include "Stack_canary.h"
//--------------------------------------------- MACROSES -------------------------------------------

#define COMMAND_LENGTH 64
#define NUM_REGIST 8
#define MAX_LABELS 67

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
    JMP = 9};

enum reg_nums {
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};



//------------------------------------------------ LABELS -----------------------------------------------------------

struct Label {
    char name[32];
    int address;
};

struct LabelsCase {
    Label labels[MAX_LABELS];
    size_t count;
};

//-------------------------------------------------- FOR READ --------------------------------------------------------------


struct Line {char* str; size_t len;};


//------------------------------------------------- REGISTERS ------------------------------------------------------

struct Registr {
    const char* name;
    int num;
    size_t len;
};

Registr registers[NUM_REGIST] = {
    {.name = "AX", .num = AX, .len = 2}, 
    {.name = "BX", .num = BX, .len = 2},
    {.name = "CX", .num = CX, .len = 2},
    {.name = "DX", .num = DX, .len = 2},
};

//---------------------------------------------- FUNCTIONS DECLARATION --------------------------------------------

char* ReadFromFile(const char* name, size_t* out_size);

Line* SplitLines(char* buffer, size_t file_size, size_t* nLines);

void Assembler(FILE* file_output, Line* index, char* buffer, size_t nLines);

void add_label(LabelsCase* Case, const char* name, size_t address);

int find_label(const LabelsCase* Case, const char* name);

int ParseLine(const char* line, int pass_num, LabelsCase* Case, FILE* file_output, size_t* ip, size_t* break_flag);
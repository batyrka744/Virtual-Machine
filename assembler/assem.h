#include "../hnik.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>


//--------------------------------------------- MACROSES -------------------------------------------

#define COMMAND_LENGTH 64

#define MAX_LABELS 69-2
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

//--------------------------------------------- CONSTS -------------------------------------------------

const char* before_ass = "Tests/square_eq.txt";
const char* after_ass = "../processor/input2.txt";
#define FILE_NAME_LENGTH 50

#define FIRST_PASS 1
#define SECOND_PASS 2
#define YES 1
#define NO 0


//------------------------------------------------ LABELS -----------------------------------------------------------

struct Label {
    char name[COMMAND_LENGTH];
    int address;
};

struct LabelsCase {   
    Label labels[MAX_LABELS];
    size_t count;               //current num of elements in tthe table
};

//-------------------------------------------------- FOR READ --------------------------------------------------------------




struct project {

    FILE* file_write;
    size_t out_size, nLines;
    char* buffer;
    Line* index;
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
    {"HLT",  HLT,  ARG_NO}, 
    {"PUSH", PUSH, ARG_VALUE},
    {"ADD",  ADD,  ARG_NO},

    {"SUB",  SUB,  ARG_NO},
    {"DIV",  DIV,  ARG_NO},
    {"OUT",  OUT,  ARG_NO},
    {"POP",  POP,  REG},
    {"MUL",  MUL,  ARG_NO},
    {"SQRT", SQRT, ARG_NO},

    {"JMP",  JMP,  LABEL},
    {"JA",   JA,   LABEL},
    {"JAE",  JAE,  LABEL},
    {"JB",   JB,   LABEL},
    {"JBE",  JBE,  LABEL},
    {"JE",   JE,   LABEL},
    {"JNE",  JNE,  LABEL},

    {"CALL", CALL, LABEL},
    {"RET",  RET,  ARG_NO},  

    {"IN",   IN,   ARG_NO},
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

void start_work(project* ass_main, AsmState* state, const char* before_ass);

void finish_work(AsmState* state, project* ass_main);
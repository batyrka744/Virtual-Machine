#include "assem.h"

int main() {
    project ass_main = {0};
    AsmState state = {0};

    start_work(&ass_main, &state);


    Assembler(&ass_main, &state);

    if (state.need_second_pass == 1) {
        state.pass = 2;
        Assembler(&ass_main, &state);
    }
    
    fwrite(state.bytecode, sizeof(StackElem_t), state.ip, ass_main.file_write);
    
       
    finish_work(&state, &ass_main);

    return 0;
}

/**
 * @brief performs one pass of the assembler
 * 
 * @param[in, out] ass_main structure with data of project
 * @param[in, out] state structure with assembler's haracteristic
 */
void Assembler(project* ass_main, AsmState* state) {
    my_assert(state != NULL,);
    my_assert(ass_main != NULL,)
    my_assert(ass_main->file_write != NULL,);
    my_assert(ass_main->buffer != NULL,);
    my_assert(ass_main->index != NULL,);
    
    state->ip = 0;
        
    for (size_t i = 0; i < ass_main->nLines; i++) {
        int res = ParseLine(ass_main->index[i].str, state);
        my_assert(res == 0,);
    }
}

/**
 * @brief parses one string
 * 
 * @param[in] line string that need to parse
 * @param[in, out] state structure with assembler's haracteristic
 * @return int 0 if ok else -1
 */
int ParseLine(const char* line, AsmState* state) {
    my_assert(line != NULL, -1);
    my_assert(state != NULL, -1);

    int offset = 0;
    char first_word[COMMAND_LENGTH] = {0};
    int readed = sscanf(line, "%s%n", first_word, &offset);

    if (first_word[0] == ';') {
        return 0;
    }
    if (readed != 1) {
        return 0;
    }
    size_t len = strlen(line);

    sscanf(line, "%s", first_word);
    char* colon = strchr(first_word, ':');
    
    if (colon != NULL) {
        if (state->pass == 1) {
            *colon = '\0';
            add_label(&state->Case, first_word, state->ip);
        }
        return 0;
    }

    CommandInfo* cmd = FindCommand(first_word);
    my_assert(cmd != NULL, -1);

    if (cmd->arg == ARG_NO) {
        int pidaras = ParseNoArg(cmd, line + offset, state);
        return pidaras;
    } else {
        int pidaras = ParseWithArg(cmd, line + offset, state);
        return pidaras;
    }

    return -1;
}

/**
 * @brief parses line with no args
 * 
 * @param[in, out] cmd 
 * @param[in] end_of_line pointer to the last readed character
 * @param[in, out] state structure with assembler's haracteristic
 * @return int 0 if ok else -1
 */
int ParseNoArg(const CommandInfo* cmd, const char* end_of_line, AsmState* state) {
    my_assert(cmd != NULL, -1);
    my_assert(state != NULL, -1);
    my_assert(end_of_line != NULL, -1);

    if (CheckRubbish(end_of_line) != 0) {
        printf(RED "Wrong ass-code: command %s expects no arguments, got '%s'\n" RESET, cmd->name, end_of_line);
        return -1;
    } 


    state->bytecode[state->ip++] = (StackElem_t)cmd->opcode;

    return 0;
}

/**
 * @brief parses line with arg
 * 
 * @param[in, out] cmd 
 * @param[in] end_of_line pointer to the last readed character
 * @param[in, out] state structure with assembler's haracteristic
 * @return int 0 if ok else -1
 */
int ParseWithArg(const CommandInfo* cmd, const char* end_of_line, AsmState* state) {
    my_assert(cmd != NULL, -1);
    my_assert(state != NULL, -1);
    my_assert(end_of_line != NULL, -1);

    if (cmd->arg == LABEL) {
        char label_name[COMMAND_LENGTH] = {0};
        int offset = 0;

        if (sscanf(end_of_line, "%s%n", label_name, &offset) != 1) {
            printf(RED "Syntax error: rubbish after label '%s'\n" RESET, label_name);
            return -1;
        } 
        if (CheckRubbish(end_of_line + offset) != 0) {
            printf(RED "Syntax Error: rubbish after label '%s'\n" RESET, label_name);
            return -1;
        }
        int target_address = find_label(&state->Case, label_name);

        if (target_address == -1) {    //метка ещё не найдена
            state->need_second_pass = 1;
            target_address = 0;
        }
        state->bytecode[state->ip] = (StackElem_t)cmd->opcode;
        state->bytecode[state->ip + 1] = (StackElem_t)target_address;
        state->ip += 2;

        return 0;
    }

    if (cmd->arg == REG) {
        char reg_name[COMMAND_LENGTH] = {0};
        int offset = 0;

        if (sscanf(end_of_line, "%s%n", reg_name, &offset) != 1) {
            printf(RED "Syntax error: rubbish after register '%s'\n" RESET, reg_name);
            return -1;
        }
        if (CheckRubbish(end_of_line + offset) != 0) {
            printf(RED "Syntax Error: rubbish after register '%s'\n" RESET, reg_name);
            return -1;
        }

        int reg_num = FindRegister((const char*)reg_name);
        if (reg_num == NOT_FOUNDED) {
            printf(RED "Invalid name of register" RESET);
        }
        state->bytecode[state->ip] = (StackElem_t)cmd->opcode;
        state->bytecode[state->ip + 1] = (StackElem_t)reg_num;
        state->ip += 2;

        return 0;
    }

    if (cmd->arg == ARG_VALUE) {

        StackElem_t num_val = 0;
        int offset = 0;
        if (sscanf(end_of_line, "%lg%n", &num_val, &offset) == 1) {
            if(CheckRubbish(end_of_line + offset) != 0) {
                printf(RED "Syntax error: rubbish after number '%lg'\n" RESET, num_val);
                return -1;
            }
            state->bytecode[state->ip] = (StackElem_t)cmd->opcode;
            state->bytecode[state->ip + 1] = (StackElem_t)num_val;
            state->ip += 2;

            return 0;
        }

        char reg_name[COMMAND_LENGTH] = {0};
        if (sscanf(end_of_line, "%s%n", reg_name, &offset) == 1) {
            if(CheckRubbish(end_of_line + offset) != 0) {
                printf(RED "Syntax error: rubbish after register '%s'\n" RESET, reg_name);
                return -1;
            }

            int reg_num = FindRegister((const char*)reg_name);
            if (reg_num == NOT_FOUNDED) {
                printf(RED "Invalid name of register" RESET);
            }
            state->bytecode[state->ip] = (StackElem_t)PUSHr;
            state->bytecode[state->ip + 1] = (StackElem_t)reg_num;
            state->ip += 2;

            return 0;
            
        } 
    }   
    return -1;
}

/**
 * @brief checks if there is rubbish after command
 * 
 * @param[in] str 
 * @return int 0 if okcelse -1
 */
int CheckRubbish(const char* str) {
    if (str == NULL) {
        return 0;
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == ';')
            return 0;
        
        if(!isspace((char)str[i])) {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief finds register in array and returns its num
 * 
 * @param[in] reg_name name of register
 * @return int register's num
 */
static int FindRegister(const char* reg_name) {
    if (reg_name == NULL)
        return NOT_FOUNDED;

    for (size_t i = 0; i < NUM_REGIST; i++) {
        if (strcmp(reg_name, registers[i].name) == 0) {
            return registers[i].num;
        }
    }
    return NOT_FOUNDED;
}




/**
 * @brief splits buffer to strings and puts pointers to them in array
 * 
 * @param[in] buffer array with text
 * @param[in] file_size size of file with text
 * @param[in, out] nLines number of the strings
 * @return Line* array with pointers to the strings and their length(needs to free)
 */
Line* SplitLines(char* buffer, size_t file_size, size_t* nLines) {

    my_assert(file_size != 0, NULL);
    my_assert(buffer != NULL, NULL); 


    Line* index = (Line*)calloc(file_size, sizeof(Line));//ass
    
    size_t str_num = 1;
    index[0].str = &buffer[0];

    
    for (size_t i = 0; i < file_size; i++) { 

        if (buffer[i] == '\n'  && i < file_size - 1) {
            buffer[i] = '\0';

            index[str_num - 1].len = (size_t)(&buffer[i] - index[str_num - 1].str);
            index[str_num++].str = &buffer[i + 1];
        }
    }
    index[str_num - 1].len = (size_t)(&buffer[file_size] - index[str_num - 1].str);

    *nLines = str_num;

    return index;
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
 * @brief adds label to table of labels
 * 
 * @param[in, out] Case structure with label's table and current number of labels
 * @param[in] name name of founded label
 * @param[in] address label's
 */
void add_label(LabelsCase* Case, const char* name, size_t address) {
    my_assert(name != NULL,);
    my_assert(Case->count < MAX_LABELS,);

    Case->labels[Case->count].address = address;
    strcpy(Case->labels[Case->count].name, name);

    (Case->count)++;
}

/**
 * @brief adds label to table of labels
 * 
 * @param[in, out] Case structure with label's table and current number of labels
 * @param[in] name name of label that need to find
 * @param[in] address label's address
 * @return int label's address if founded else -1
 */
int find_label(const LabelsCase* Case, const char* name) {
    my_assert(Case != NULL, -1);
    my_assert(name != NULL, 0);

    for (size_t i = 0; i < Case->count; i++) {
        if (strcmp(name, Case->labels[i].name) == 0) {
            return Case->labels[i].address;
        }
    }
    return NOT_FOUNDED;
}

/**
 * @brief find command by its name
 * 
 * @param[in] name name of command 
 * @return CommandInfo* pointer on structure with command's characteristic
 */
CommandInfo* FindCommand(const char* name) {
    my_assert(name != 0, NULL);
    size_t size_arr = sizeof(all_commands) / sizeof(CommandInfo);

    for (size_t i = 0; i < size_arr; i++) {
        if (strcmp(all_commands[i].name, name) == 0) {
            return all_commands + i;
        }
    }
    return NULL;
}

/**
 * @brief finishes work
 * 
 * @param[in, out] state pointer on structure with assembler's characteristic
 * @param[in, out] ass_main pointer on structure with project data
 */
void finish_work(AsmState* state, project* ass_main) {
    my_assert(state != NULL,);
    my_assert(ass_main != NULL,);
    my_assert(ass_main->file_write != NULL,);

    fclose(ass_main->file_write); 

    free(state->bytecode);
    free(ass_main->index);
    free(ass_main->buffer); 
}

/**
 * @brief starts work with processor
 * 
 * @param[in, out] ass_main pointer on structure with project data
 * @param[in, out] state pointer on structure with assembler's characteristic
 */
void start_work(project* ass_main, AsmState* state) {
    my_assert(ass_main != NULL,);
    ass_main->file_write = fopen(after_ass, "wb");
    my_assert(ass_main->file_write != NULL,);

    ass_main->buffer = ReadFromFile(before_ass, &ass_main->out_size);   
    ass_main->index = SplitLines(ass_main->buffer, ass_main->out_size, &ass_main->nLines);

    state->pass = 1;
    state->bytecode = (StackElem_t*)calloc(ass_main->nLines*2, sizeof(StackElem_t));
}
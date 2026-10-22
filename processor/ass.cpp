#include "assem.h"

//massiv structur enum colvo args blyat pizda mne

int main() {
    FILE* file_write = fopen(after_ass, "w");
    my_assert(file_write != NULL, 0);

    size_t out_size = 0; 
    size_t nLines = 0;

    char* buffer = ReadFromFile(before_ass, &out_size);   //struct и free
    Line* index = SplitLines(buffer, out_size, &nLines);

    Assembler(file_write, index, buffer, nLines);
    
    fclose(file_write);      //free zdes
    return 0;
}

void Assembler(FILE* file_output, Line* index, char* buffer, size_t nLines) {
    my_assert(file_output != NULL,);


    LabelsCase Case = {0};     //два раза вызвать ассемблер вместо for


    size_t break_flag = 0;
    size_t ip = 0;

    for (size_t j = 1; j < 3; j++) {
        ip = 0;
        for (size_t i = 0; i < nLines && break_flag == 0; i++) {
            ParseLine(index[i].str, j, &Case, file_output, &ip, &break_flag);
        }
        break_flag = 0;
    }
    

    free(index);
    free(buffer);
}


int ParseLine(const char* line, int pass_num, LabelsCase* Case, FILE* file_output, size_t* ip, size_t* break_flag) {
    size_t len = strlen(line); //pass_num?
    //return error
    char label_name[COMMAND_LENGTH] = {0};

    sscanf(line, "%s", label_name);
    char* colon = strchr(label_name, ':');//line
    my_assert(!isalpha(*(colon + 1)) || !isdigit(*(colon + 1)), -1);  

    if (colon != NULL) {
        if (pass_num == 1) {
            *colon = '\0';
            add_label(Case, label_name, *ip);
        }
        return 0;

    } 

    char command_name[COMMAND_LENGTH] = {0};
    char arg_str[COMMAND_LENGTH] = {0};

    int get_command = sscanf(line, "%s", command_name);  
//PISI COMENTARII
    if (get_command != 1) { 
        return 0;
    }

    if (strcmp(command_name, "PUSH") == 0) {    //func for 2 and func for 1 arg

        StackElem_t value = 0;
        if (sscanf(line, "%*s %lg", &value) == 1)   //comment and ubrat else
            WITH_ARGS(PUSH, value);

        else if (sscanf(line, "%*s %s", arg_str) == 1) {
            int check_valid = 0;

            for (size_t j = 0; j < NUM_REGIST; j++) {
                if (strcmp(arg_str, registers[j].name) == 0) {  
                    WITH_ARGS(PUSHr, registers[j].num);
                    check_valid++;
                    break;
                } 
            }
            my_assert(check_valid > 0, -1);       //if name of register is wrong

        } else
            my_assert(0, -1);


    } else if (strcmp(command_name, "ADD") == 0) {   //считать все и написать один раз fwrite   не придется в maine otkrivat
        NO_ARGS(ADD);    

    } else if (strcmp(command_name, "SUB") == 0) {
        NO_ARGS(SUB);

    } else if (strcmp(command_name, "MUL") == 0) {
        NO_ARGS(MUL);

    } else if (strcmp(command_name, "DIV") == 0) {
        NO_ARGS(DIV);

    } else if (strcmp(command_name, "OUT") == 0) {
        NO_ARGS(OUT);

    } else if (strcmp(command_name, "HLT") == 0) {
        NO_ARGS(HLT); 
        if (pass_num == 2)
            (*break_flag)++;
        

    } else if (strcmp(command_name, "POP") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);
        int check_valid = 0;

        for (size_t j = 0; j < NUM_REGIST; j++) {
            if (strcmp(arg_str, registers[j].name) == 0) {
                WITH_ARGS(POP, registers[j].num);
                check_valid++;
                break;
            } 
        }
        my_assert(check_valid > 0, -1);


//------------------------------------------------------ JUMP FAMILY ---------------------------------------------------
    } else if (strcmp(command_name, "JMP") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JMP, address);

    } else if (strcmp(command_name, "JA") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JA, address);

    } else if (strcmp(command_name, "JAE") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JAE, address);

    } else if (strcmp(command_name, "JB") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JB, address);

    } else if (strcmp(command_name, "JBE") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JMP, address);

    } else if (strcmp(command_name, "JE") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JMP, address);

    } else if (strcmp(command_name, "JNE") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(JNE, address);
    
//-----------------------------------------------------------------------------------------------------------------------

    } else if (strcmp(command_name, "CALL") == 0) {

        my_assert(sscanf(line, "%*s %s", arg_str), -1);

        int address = 0;
        if (pass_num == 2) {
            address = find_label(Case, arg_str);
            my_assert(address != -1, -1);
        }
        WITH_ARGS(CALL, address);

    } else if (strcmp(command_name, "RET") == 0) {
        NO_ARGS(RET);
    
    } else if (strcmp(command_name, "") == 0) {
        NO_ARGS(IN);

    } else {
        printf(RED "Assembler Error: wrong command's name\n" RESET);
        (*break_flag)++;
    }
    
    return 0;
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

void add_label(LabelsCase* Case, const char* name, size_t address) {
    my_assert(name != NULL,);
    my_assert(Case->count < MAX_LABELS,);

    Case->labels[Case->count].address = address;
    strcpy(Case->labels[Case->count].name, name);

    (Case->count)++;
}

int find_label(const LabelsCase* Case, const char* name) {
    my_assert(Case != NULL, -1);
    my_assert(name != NULL, 0);

    for (size_t i = 0; i < Case->count; i++) {
        if (strcmp(name, Case->labels[i].name) == 0) {
            return Case->labels[i].address;
        }
    }
    return -1;
}
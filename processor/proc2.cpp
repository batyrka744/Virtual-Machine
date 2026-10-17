#include "process2.h"

//backups
//udalit staroe


int main() {

    clean_file();  

    proccessor intel = {0};
    size_t buffer_size = 0;
    
    intel.code = lf_buffer(after_ass, &buffer_size);  
    assert(intel.code != NULL);

    intel.buffer_size = buffer_size;

    ReadFromBuffer(&intel);

    free(intel.code);
    return 0;
}

void ReadFromBuffer(proccessor* v_m) {
    assert(v_m != NULL);

    STACK_INIT(v_m->stk, 1);

    int command = 0;

    errors err = ERR_OK;
    size_t break_flag = 0;

    StackElem_t a = 0;
    StackElem_t b = 0;

    StackElem_t popped = 0;

    while (v_m->pc < v_m->buffer_size && break_flag == 0) {

        CodeDump(v_m);

        command = (int)v_m->code[v_m->pc];
        
        switch(command) {

            case HLT:
                StackDestroy(&v_m->stk); 
                break_flag++;
                v_m->pc += 1;
                break;

            case PUSH:
                StackPush(&v_m->stk, v_m->code[v_m->pc + 1]);
                v_m->pc += 2;
                break;

            case ADD:
                b = StackPop(&v_m->stk, &err);
                a = StackPop(&v_m->stk, &err);
                StackPush(&v_m->stk, a + b);
                v_m->pc += 1;
                break;

            case SUB:
                b = StackPop(&v_m->stk, &err); 
                a = StackPop(&v_m->stk, &err); 
                StackPush(&v_m->stk, a - b);
                v_m->pc += 1;
                break;

            case DIV:
                b = StackPop(&v_m->stk, &err);
                a = StackPop(&v_m->stk, &err);
                StackPush(&v_m->stk, a / b);
                v_m->pc += 1;
                break;

            case OUT:
                printf(GREEN "%lg\n" RESET, StackPop(&v_m->stk, &err));
                v_m->pc += 1;
                break;

            case POP:
                popped = StackPop(&v_m->stk, &err);
                v_m->registers[(int)v_m->code[v_m->pc + 1]] = popped;
                v_m->pc += 2;
                break;

            case PUSHr:
                StackPush(&v_m->stk, (int)v_m->code[v_m->pc]);
                v_m->pc += 1;
                break;

            case MUL:
                b = StackPop(&v_m->stk, &err);
                a = StackPop(&v_m->stk, &err);
                StackPush(&v_m->stk, a * b);
                v_m->pc += 1;
                break;

            case JMP:
                v_m->pc = (int)v_m->code[v_m->pc + 1];
                break;
                
            default:
                printf("switch-case err\n");
                v_m->pc += 1;
                break;
        }
    }
    my_assert(err != ERR_OK,);
}



void CodeDump(proccessor* v_m) {
    
    FILE* file = fopen(LOG_FILE, "a");
    my_assert(file != NULL,);

    size_t empty_flag = 0;
    size_t arg_flag = 0;

    fprintf(file, "========= BYTE-CODE =========\n");
    for (size_t i = 0; i < v_m->buffer_size, empty_flag < 2; i++) {

        fprintf(file, "[%llu] %lg", i, v_m->code[i]);

        const char* name = CommandName((int)v_m->code[i], &arg_flag);
        if (*name != '\0') 
            fprintf(file, "   (%s)", name);

        if (i == v_m->pc) 
            fprintf(file, "      [<--------");
        
        fprintf(file, "\n");

        if (v_m->code[i] == 0) empty_flag++;
    }

    fprintf(file, "\n");
    fclose(file);
}


void clean_file(void) {
    fclose(fopen(LOG_FILE, "w"));
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


    Line* index = (Line*)calloc(file_size, sizeof(Line));
    
    size_t str_num = 1;
    index[0].str = &buffer[0];

    
    for (size_t i = 0; i < file_size; i++) { //mb uprostit

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

double* lf_buffer(const char* name, size_t* buffer_size) {

    my_assert(name != NULL, NULL);
    my_assert(buffer_size != NULL, NULL);

    size_t outsize = 0;
    size_t nLines = 0;

    char* buffer = ReadFromFile(name, &outsize);
    my_assert(buffer != NULL, NULL);

    Line* index = SplitLines(buffer, outsize, &nLines);
    my_assert(index != NULL, NULL);

    double* code = (double*)calloc(outsize, sizeof(double));  //?????????
    my_assert(code != NULL, NULL);

    *buffer_size = outsize;

    size_t j = 0;

    for (size_t i = 0; i < nLines; i++) {

        double command = 0;
        double num = 0;

        int readed = sscanf(index[i].str, "%lf %lf\n", &command, &num);

        switch(readed) {

            case 0:
                break;

            case 1:
                code[j++] = command;
                break;

            case 2:
                code[j++] = command;
                code[j++] = num;
                break;

            default:
                my_assert(0, NULL);
                break;
        }
    }

    return code;
}

const char* CommandName(int command, size_t* arg_flag) {

    *arg_flag = 0;

    switch(command) {

        case HLT:
            return "HLT";
        case PUSH:
            *arg_flag = 1;
            return "PUSH";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case DIV:
            return "DIV";
        case OUT:
            return "OUT";
        case POP:
            *arg_flag = 1;
            return "POP";
        case PUSHr:
            *arg_flag = 1;
            return "PUSHr";
        case MUL:
            return "MUL";
        case JMP:
            *arg_flag = 1;
            return "JMP";
        default:
            return "";
    }

    return "";
}
#include "process.h"


int main() {

    clean_file();  

    proccessor intel = {0};
    
    intel.code = lf_buffer(after_ass, &intel.buffer_size);  
    my_assert(intel.code != NULL, -1);  

    ReadFromBuffer(&intel);

    DestroyIntel(&intel);
    return 0;
}

void ReadFromBuffer(proccessor* v_m) {
    my_assert(v_m != NULL,);  

    STACK_INIT(v_m->stk, STACK_CAPACITY);   
    STACK_INIT(v_m->ret_stk, RECURSION_DEEPTH);    

    int command = 0;

    errors err = ERR_OK;
    size_t break_flag = 0;

    StackElem_t a = 0;
    StackElem_t b = 0;

    StackElem_t popped = 0;

    while ((v_m->pc < v_m->buffer_size) && (break_flag == 0)) {   

        CodeDump(v_m);

        command = (int)v_m->code[v_m->pc];
        
        execution(v_m, command, &break_flag, &err);
        my_assert(err == ERR_OK,);
    }
}
    

void execution(proccessor* v_m, int command, size_t* break_flag, errors* err) {

    StackElem_t a = 0;
    StackElem_t b = 0;
    

    switch(command) {

        case HLT:
            (*break_flag)++;
            v_m->pc += 1;
            break;

        case PUSH:
            StackPush(&v_m->stk, v_m->code[v_m->pc + 1]);
            v_m->pc += 2;
            break;

        case ADD: 
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);
            StackPush(&v_m->stk, a + b);
            v_m->pc += 1;
            break;

        case SUB:
            b = StackPop(&v_m->stk, err); 
            a = StackPop(&v_m->stk, err); 
            StackPush(&v_m->stk, a - b);
            v_m->pc += 1;
            break;

        case DIV:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);
            StackPush(&v_m->stk, a / b);
            v_m->pc += 1;
            break;

        case OUT:
            printf(GREEN "%lg\n" RESET, StackPop(&v_m->stk, err));
            v_m->pc += 1;
            break;

        case POP: {
            v_m->registers[(int)v_m->code[v_m->pc + 1]] = StackPop(&v_m->stk, err);
            v_m->pc += 2;
            break;
        }
            

        case PUSHr:
            StackPush(&v_m->stk, v_m->registers[(int)v_m->code[v_m->pc + 1]]);
            v_m->pc += 2;
            break;

        case MUL:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);
            StackPush(&v_m->stk, a * b);
            v_m->pc += 1;
            break;

        case JMP:
            v_m->pc = (int)v_m->code[v_m->pc + 1];
            break;

        case JA:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (b > a) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;

            break;

        case JAE:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (b >= a) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;
            
            break;

        case JB:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (a > b) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;
            
            break;

        case JBE:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (a >= b) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;
            
            break;

        case JE:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (b == a) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;
            
            break;

        case JNE:
            b = StackPop(&v_m->stk, err);
            a = StackPop(&v_m->stk, err);

            if (b != a) 
                v_m->pc = (int)v_m->code[v_m->pc + 1];
            else
                v_m->pc += 2;
            
            break;

        case CALL:
            StackPush(&v_m->ret_stk, v_m->pc + 2);
            v_m->pc = (int)v_m->code[v_m->pc + 1];
            break;
            
        case RET:
            v_m->pc = (int)StackPop(&v_m->ret_stk, err);
            break;

        case IN: {
            StackElem_t x = 0;

            printf(MAGENTA "Enter a value\n" RESET);
            fflush(stdout);
            my_assert(scanf("%lg", &x) == 1,)

            StackPush(&v_m->stk, x);
            v_m->pc += 1;
            break;
        }
        
        default:
            printf("switch-case err\n");
            v_m->pc += 1;
            break;
    }
}





void CodeDump(proccessor* v_m) {
    my_assert(v_m != NULL,);
    
    FILE* file = fopen(LOG_FILE, "a");
    my_assert(file != NULL,);

    size_t empty_flag = 0;
    size_t arg_flag = 0;

    fprintf(file, "========= BYTE-CODE =========\n");
    for (size_t i = 0; i < v_m->buffer_size && empty_flag < 2; i++) {

        fprintf(file, "[%llu] %lg", i, v_m->code[i]);

        if (arg_flag == 1) {
            arg_flag = 0;
        } else {
            const char* name = CommandName((int)v_m->code[i], &arg_flag);
            if (*name != '\0') 
                fprintf(file, "   (%s)", name);
        }
        
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
    char* buffer = ReadFromFile(name, &outsize);
    my_assert(buffer != NULL, NULL);

    *buffer_size = outsize / sizeof(StackElem_t);

    return (StackElem_t*)buffer;
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

        case JA:
            *arg_flag = 1;
            return "JA";

        case JAE:
            *arg_flag = 1;
            return "JAE";

        case JB:
            *arg_flag = 1;
            return "JB";

        case JBE:
            *arg_flag = 1;
            return "JBE";

        case JE:
            *arg_flag = 1;
            return "JNE";

        case JNE:
            *arg_flag = 1;
            return "JNE";

        case CALL:
            *arg_flag = 1;
            return "CALL";

        case RET:
            return "RET";

        case IN:
            return "IN";

        default:
            return "";
    }

    return "";
}

void DestroyIntel(proccessor* intel) {

    StackDestroy(&intel->stk);
    StackDestroy(&intel->ret_stk);
    intel->buffer_size = POISON_SIZE;
    intel->pc = POISON_SIZE;

    free(intel->code);
}
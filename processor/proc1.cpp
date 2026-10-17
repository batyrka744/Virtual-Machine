#include "process.h"

#define STACK_DEBUG

#define INPUT "input1.txt"
#define COMMAND_LENGTH 10



int main() {
    //FILE* file = fopen("C:/Users/Admin/OneDrive/Desktop/C/RT/huawei/Proccessor/processor/input.txt", "r");
    FILE* file = fopen("Proccessor\\processor\\input1.txt", "r");
    //FILE* file = fopen(INPUT, "r");
    //printf("%s\n", strerror(errno));
    printf("%p\n", file);
    assert(file != NULL);

    stack_t stk = {};
    
    printf("MIU\n");

    ReadFromFile(file, &stk);


    fclose(file);

    return 0;
}

int ReadFromFile(FILE* file, stack_t* stk) {

    assert(file != NULL);
    STACK_INIT(*stk, 10);
    

    char s[COMMAND_LENGTH] = {};
    StackElem_t value = 0;
    int get_command = fscanf(file, "%s %d", s, &value);

    errors err = ERR_OK;

    while (get_command != EOF) {
            
        if (get_command == 2) {
            my_assert(strcmp(s, "PUSH") == 0, -1);
            StackPush(stk, value); 
            
            
        } else if (get_command == 1) {

            if (strcmp(s, "POP") == 0) {
                StackElem_t popped = StackPop(stk, &err);

            } else if (strcmp(s, "ADD") == 0) {
                StackPush(stk, StackPop(stk, &err) + StackPop(stk, &err)); 

            } else if (strcmp(s, "SUB") == 0) {
                StackElem_t b = StackPop(stk, &err); 
                StackElem_t a = StackPop(stk, &err); 
                StackPush(stk, a - b);

            } else if (strcmp(s, "MUL") == 0) {
                StackPush(stk, StackPop(stk, &err) * StackPop(stk, &err));

            } else if (strcmp(s, "DIV") == 0) {
                StackElem_t b = StackPop(stk, &err);
                StackElem_t a = StackPop(stk, &err);
                StackPush(stk, a / b);

            } else if (strcmp(s, "OUT") == 0) {
                printf(GREEN "%lg\n" RESET, StackPop(stk, &err));

            } else if (strcmp(s, "HLT") == 0) {
                StackDestroy(stk); 
                break;

            } else {
                printf("Хуйня\n");
                break;
            }

        } else {
            printf("Ты гей\n");
            break;
        }

        get_command = fscanf(file, "%s %d", s, &value);    
    }
    
    return 0;
}

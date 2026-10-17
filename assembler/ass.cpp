#include "assem.h"

enum commands {HLT = 0, PUSH = 1, ADD = 2, SUB = 3, DIV = 4, OUT = 5, POP = 6, PUSHr = 7, MUL = 8, JMP = 9};

int Assembler(const char* name, FILE* file_output) {
    assert(name != NULL);
    assert(file_output != NULL);

    size_t out_size = 0; 
    size_t nLines = 0;

    char* buffer = ReadFromFile(name, &out_size);
    Line* index = SplitLines(buffer, out_size, &nLines);

    char s[COMMAND_LENGTH] = {};
    int value = 0;

    for (size_t i = 0; i < nLines; i++) {
        int get_command = sscanf(index[i].str, "%63s %d", s, &value);

        if (get_command == EOF) {
            continue; // пустая строка между командами
        }

        if (get_command == 2) {
            my_assert(strcmp(s, "PUSH") == 0, -1);
            fprintf(file_output, "%d %d\n", 1, value); 
            
        } else if (get_command == 1) {

            if (strcmp(s, "POP") == 0) {
                fprintf(file_output, "%d\n", POP);

            } else if (strcmp(s, "ADD") == 0) {
                fprintf(file_output, "%d\n", ADD);

            } else if (strcmp(s, "SUB") == 0) {
                fprintf(file_output, "%d\n", SUB);

            } else if (strcmp(s, "MUL") == 0) {
                fprintf(file_output, "%d\n", MUL);

            } else if (strcmp(s, "DIV") == 0) {
                fprintf(file_output, "%d\n", DIV);

            } else if (strcmp(s, "OUT") == 0) {
                fprintf(file_output, "%d\n", OUT);

            } else if (strcmp(s, "HLT") == 0) {
                fprintf(file_output, "%d\n", HLT); 
                break;

            } else {
                printf(RED "Disassembler Error: wrong command's name\n" RESET);
                break;
            }

        } else {
            printf(RED "Assembler Error: wrong number of words\n" RESET);
            break;
        }
    }

    free(index);
    free(buffer);

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


    Line* index = (Line*)calloc(file_size, sizeof(Line));
    
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

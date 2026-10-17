#include "assem.h"



int main() {
    FILE* file_write = fopen(after_ass, "w");
    my_assert(file_write != NULL, 0);


    size_t out_size = 0; 
    size_t nLines = 0;

    char* buffer = ReadFromFile(before_ass, &out_size);
    Line* index = SplitLines(buffer, out_size, &nLines);

    Assembler(file_write, index, buffer, nLines);
    
    fclose(file_write);
    return 0;
}

void Assembler(FILE* file_output, Line* index, char* buffer, size_t nLines) {
    my_assert(file_output != NULL,);


    for (size_t i = 0; i < nLines; i++) {

        char command_name[COMMAND_LENGTH] = {0};
        char arg_str[COMMAND_LENGTH] = {0};
        double value = 0;

        int get_command = sscanf(index[i].str, "%63s %lg", command_name, &value);

        if (get_command == EOF) {
            continue; // пустая строка между командами
        }

        if (get_command == 2) {

            if (strcmp(command_name, "PUSH") == 0) {
                fprintf(file_output, "%d %lg ", PUSH, value); 

            } else if (strcmp(command_name, "JMP") == 0) {
                fprintf(file_output, "%d %d ", JMP, (int)value);

            } else {
                my_assert(0,);
            }
            
        } else if (get_command == 1) {

            if (strcmp(command_name, "ADD") == 0) {
                fprintf(file_output, "%d ", ADD);     

            } else if (strcmp(command_name, "SUB") == 0) {
                fprintf(file_output, "%d ", SUB);

            } else if (strcmp(command_name, "MUL") == 0) {
                fprintf(file_output, "%d ", MUL); 

            } else if (strcmp(command_name, "DIV") == 0) {
                fprintf(file_output, "%d ", DIV);

            } else if (strcmp(command_name, "OUT") == 0) {
                fprintf(file_output, "%d ", OUT);

            } else if (strcmp(command_name, "HLT") == 0) {
                fprintf(file_output, "%d ", HLT); 
                break;


            } else if (strcmp(command_name, "PUSH") == 0) {

                sscanf(index[i].str, "%s %s", command_name, arg_str);
                int check_valid = 0;

                for (size_t j = 0; j < NUM_REGIST; j++) {
                    if (strcmp(arg_str, registers[j].name) == 0) {
                        fprintf(file_output, "%d %d ", PUSHr, registers[j].num);
                        check_valid++;
                        break;
                    } 
                }
                my_assert(check_valid > 0,);


            } else if (strcmp(command_name, "POP") == 0) {

                sscanf(index[i].str, "%s %s", command_name, arg_str);
                int check_valid = 0;

                for (size_t j = 0; j < NUM_REGIST; j++) {
                    if (strcmp(arg_str, registers[j].name) == 0) {
                        fprintf(file_output, "%d %d ", POP, registers[j].num);
                        check_valid++;
                        break;
                    } 
                }
                my_assert(check_valid > 0,);


            } else {
                printf(RED "Assembler Error: wrong command's name\n" RESET);
                break;
            }

        } else {
            printf(RED "Assembler Error: wrong number of words or empty line\n" RESET);
            break;
        }
    } // end of assembler's for 

    free(index);
    free(buffer);
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

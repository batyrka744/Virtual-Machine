#include "assem.h"

const char* asm_src = "Proccessor\\processor\\input2.txt";
const char* bin_dst = "Proccessor\\processor\\input1.txt";


int main() {
    FILE* file_write = fopen(bin_dst, "w");
    assert(file_write != NULL);
    DisAssembler(asm_src, file_write);
    fclose(file_write); 

    return 0;
}


int DisAssembler(const char* name, FILE* file_output) {
    assert(name != NULL);
    assert(file_output != NULL);

    size_t out_size = 0; 
    size_t nLines = 0;

    char* buffer = ReadFromFile(name, &out_size);
    Line* index = SplitLines(buffer, out_size, &nLines);

    int s = 0;
    int value = 0;

    for (size_t i = 0; i < nLines; i++) {
        int get_command = sscanf(index[i].str, "%d %d", s, &value);

        if (get_command == EOF) {
            continue; // пустая строка между командами
        }

        int break_flag = 0;

        if (get_command == 2) {
            my_assert(s == PUSH, -1);
            fprintf(file_output, "%d %d\n", 1, value); 
            
        } else if (get_command == 1) {

            switch(s) {

                case HLT:
                    fprintf(file_output, "HLT\n");
                    break_flag++;
                    break;

                case ADD:
                    fprintf(file_output, "ADD\n");
                    break;

                case SUB:
                    fprintf(file_output, "SUB\n");
                    break;

                case DIV:
                    fprintf(file_output, "DIV\n");
                    break;

                case OUT:
                    fprintf(file_output, "OUT\n");
                    break;

                // case POP:
                //     fprintf(file_output, "POP\n");
                //     break;

                // case PUSHr:
                //     fprintf(file_output, "PUSHr\n");
                //     break;

                case MUL:
                    fprintf(file_output, "MUL\n");
                    break;

                // case JMP:
                //     fprintf(file_output, "JMP\n");
                //     break;

                default:
                    printf(RED "Disassembler Error: wrong command's name\n" RESET);
                    break_flag++;
                    break;


            }

            if (break_flag > 0) {
                break;
            }

        } else {
            printf(RED "Disassembler Error: wrong number of words\n" RESET);
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

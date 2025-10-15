#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TYPE int64_t
#define TYPED(THING) i_##THING
#include "list.c"

char input_path[] = "inputs/02.txt";

void get_program(FILE* fp, struct i_list* program) {
    int64_t val;
    while (fscanf(fp, "%li", &val) != -1) {
        i_list_append(program, val);
        fgetc(fp); // eat comma
    }
}

bool run_program(struct i_list* program) {
    for (size_t i = 0; i < program->length; i++) {
        int64_t opcode = i_list_get(program, i);
        if (opcode == 1) {
            // Add
            int64_t src1 = i_list_get(program, ++i);
            int64_t src2 = i_list_get(program, ++i);
            int64_t dst = i_list_get(program, ++i);
            int64_t result = i_list_get(program, src1) + i_list_get(program, src2);
            i_list_set(program, dst, result);
        } else if (opcode == 2) {
            // Multiply
            int64_t src1 = i_list_get(program, ++i);
            int64_t src2 = i_list_get(program, ++i);
            int64_t dst = i_list_get(program, ++i);
            int64_t result = i_list_get(program, src1) * i_list_get(program, src2);
            i_list_set(program, dst, result);
        } else if (opcode == 99) {
            // Halt
            return true;
        } else {
            printf("Unknown opcode: %li\n", opcode);
            exit(1);
        }
    }
    return false;
}

int64_t part1(void) {
    FILE* fp = fopen(input_path, "r");

    struct i_list program = i_list_init();
    get_program(fp, &program);

    i_list_set(&program, 1, 12);
    i_list_set(&program, 2, 2);
    run_program(&program);
    int64_t result = i_list_get(&program, 0);

    i_list_free(&program);
    fclose(fp);
    return result;
}

int64_t part2(void) {
    FILE* fp = fopen(input_path, "r");

    struct i_list program = i_list_init();
    get_program(fp, &program);

    int64_t i, j;
    for (i = 0; i <= 99; i++) {
        for (j = 0; j <= 99; j++) {
            struct i_list cloned = i_list_clone(&program);
            i_list_set(&cloned, 1, i);
            i_list_set(&cloned, 2, j);
            bool exited = run_program(&cloned);
            int64_t result = i_list_get(&cloned, 0);
            i_list_free(&cloned);
            if (exited && result == 19690720) {
                goto exit_loop;
            }
        }
    }
exit_loop:

    i_list_free(&program);
    fclose(fp);
    return i * 100 + j;
}

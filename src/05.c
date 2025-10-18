#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TYPE int64_t
#define TYPED(THING) i_##THING
#include "list.c"

extern char input_path[];

void get_program(FILE* fp, struct i_list* program) {
    int64_t val;
    while (fscanf(fp, "%li", &val) != -1) {
        i_list_append(program, val);
        fgetc(fp); // Discard comma
    }
}

uint8_t get_parameter_mode(int64_t inst, uint8_t index) {
    inst /= 100;
    for (uint8_t i = 0; i < index; i++) {
        inst /= 10;
    }
    return inst % 10;
}

int64_t resolve_parameter(struct i_list* program, int64_t param, uint8_t mode) {
    if (mode == 0) {
        // Position mode
        return i_list_get(program, param);
    } else if (mode == 1) {
        // Immediate mode
        return param;
    } else {
        printf("Unknown parameter mode: %u\n", mode);
        exit(1);
    }
}

bool run_program(struct i_list* program, struct i_list* inputs, struct i_list* outputs) {
    for (size_t i = 0; i < program->length; i++) {
        int64_t inst = i_list_get(program, i);
        int64_t opcode = inst % 100;
        switch (opcode) {
        case 1: {
            // Add
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            int64_t dst = i_list_get(program, ++i);
            int64_t result =
                resolve_parameter(program, src1, mode1) + resolve_parameter(program, src2, mode2);
            i_list_set(program, dst, result);
            break;
        }
        case 2: {
            // Multiply
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            int64_t dst = i_list_get(program, ++i);
            int64_t result =
                resolve_parameter(program, src1, mode1) * resolve_parameter(program, src2, mode2);
            i_list_set(program, dst, result);
            break;
        }
        case 3: {
            // Input
            int64_t dst = i_list_get(program, ++i);
            int64_t input = i_list_remove(inputs, 0);
            i_list_set(program, dst, input);
            break;
        }
        case 4: {
            // Output
            int64_t src = i_list_get(program, ++i);
            uint8_t mode = get_parameter_mode(inst, 0);
            int64_t output = resolve_parameter(program, src, mode);
            i_list_append(outputs, output);
            break;
        }
        case 5: {
            // Jump-if-true
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            if (resolve_parameter(program, src1, mode1) != 0) {
                i = resolve_parameter(program, src2, mode2) - 1;
            }
            break;
        }
        case 6: {
            // Jump-if-false
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            if (resolve_parameter(program, src1, mode1) == 0) {
                i = resolve_parameter(program, src2, mode2) - 1;
            }
            break;
        }
        case 7: {
            // Less than
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            int64_t dst = i_list_get(program, ++i);
            int64_t result =
                resolve_parameter(program, src1, mode1) < resolve_parameter(program, src2, mode2)
                    ? 1
                    : 0;
            i_list_set(program, dst, result);
            break;
        }
        case 8: {
            // Equals
            int64_t src1 = i_list_get(program, ++i);
            uint8_t mode1 = get_parameter_mode(inst, 0);
            int64_t src2 = i_list_get(program, ++i);
            uint8_t mode2 = get_parameter_mode(inst, 1);
            int64_t dst = i_list_get(program, ++i);
            int64_t result =
                resolve_parameter(program, src1, mode1) == resolve_parameter(program, src2, mode2)
                    ? 1
                    : 0;
            i_list_set(program, dst, result);
            break;
        }
        case 99:
            // Halt
            return true;
        default:
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
    fclose(fp);

    struct i_list inputs = i_list_init();
    struct i_list outputs = i_list_init();

    i_list_append(&inputs, 1);
    run_program(&program, &inputs, &outputs);
    int64_t result = i_list_get(&outputs, outputs.length - 1);

    i_list_free(&outputs);
    i_list_free(&inputs);
    i_list_free(&program);
    return result;
}

int64_t part2(void) {
    FILE* fp = fopen(input_path, "r");
    struct i_list program = i_list_init();
    get_program(fp, &program);
    fclose(fp);

    struct i_list inputs = i_list_init();
    struct i_list outputs = i_list_init();

    i_list_append(&inputs, 5);
    run_program(&program, &inputs, &outputs);
    int64_t result = i_list_get(&outputs, outputs.length - 1);

    i_list_free(&outputs);
    i_list_free(&inputs);
    i_list_free(&program);
    return result;
}

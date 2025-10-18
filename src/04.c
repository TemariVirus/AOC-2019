#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern char input_path[];

int64_t part1(void) {
    FILE* fp = fopen(input_path, "r");
    int64_t lower, upper;
    fscanf(fp, "%lu", &lower);
    fgetc(fp); // Discard '-'
    fscanf(fp, "%lu", &upper);
    fclose(fp);

    int64_t matches = 0;
    for (int64_t i = lower; i <= upper; i++) {
        bool has_adjacent = false;
        int64_t last_digit = -1;
        for (int64_t base = 100000; base > 0; base /= 10) {
            int64_t digit = (i / base) % 10;
            if (digit < last_digit) {
                goto loop_end;
            }
            if (digit == last_digit) {
                has_adjacent = true;
            }
            last_digit = digit;
        }
        if (has_adjacent) {
            matches++;
        }
    loop_end:
        continue;
    }

    return matches;
}

int64_t part2(void) {
    FILE* fp = fopen(input_path, "r");
    int64_t lower, upper;
    fscanf(fp, "%lu", &lower);
    fgetc(fp); // Discard '-'
    fscanf(fp, "%lu", &upper);
    fclose(fp);

    int64_t matches = 0;
    for (int64_t i = lower; i <= upper; i++) {
        bool has_double = false;
        int64_t adjacent_count = 1, last_digit = -1;
        for (int64_t base = 100000; base > 0; base /= 10) {
            int64_t digit = (i / base) % 10;
            if (digit < last_digit) {
                goto loop_end;
            }
            if (digit == last_digit) {
                adjacent_count++;
            } else {
                if (adjacent_count == 2) {
                    has_double = true;
                }
                adjacent_count = 1;
            }
            last_digit = digit;
        }
        if (adjacent_count == 2) {
            has_double = true;
        }

        if (has_double) {
            matches++;
        }
    loop_end:
        continue;
    }

    return matches;
}

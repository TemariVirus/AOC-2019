#include <stdint.h>
#include <stdio.h>

extern char input_path[];

int64_t part1(void) {
    FILE* fp = fopen(input_path, "r");
    int64_t sum = 0, mass;
    while (fscanf(fp, "%li", &mass) != -1) {
        sum += (mass / 3) - 2;
    }
    fclose(fp);
    return sum;
}

int64_t part2(void) {
    FILE* fp = fopen(input_path, "r");
    int64_t sum = 0, mass;
    while (fscanf(fp, "%li", &mass) != -1) {
        mass = (mass / 3) - 2;
        while (mass > 0) {
            sum += mass;
            mass = (mass / 3) - 2;
        }
    }
    fclose(fp);
    return sum;
}

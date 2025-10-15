#include <stdint.h>
#include <stdio.h>

uint64_t part1(void) {
    FILE* fptr = fopen("inputs/01.txt", "r");
    uint64_t sum = 0, mass;
    while (fscanf(fptr, "%lu", &mass) != -1) {
        sum += (mass / 3) - 2;
    }
    fclose(fptr);
    return sum;
}

uint64_t part2(void) {
    FILE* fptr = fopen("inputs/01.txt", "r");
    uint64_t sum = 0;
    int64_t mass;
    while (fscanf(fptr, "%li", &mass) != -1) {
        mass = (mass / 3) - 2;
        while (mass > 0) {
            sum += mass;
            mass = (mass / 3) - 2;
        }
    }
    fclose(fptr);
    return sum;
}

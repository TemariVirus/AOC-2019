#include <stdint.h>
#include <stdio.h>
#include <time.h>

extern uint64_t part1(void);
extern uint64_t part2(void);

void print_duration(struct timespec start, struct timespec end) {
    double total_seconds = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if (total_seconds < 1e-6) {
        printf("Time: %.0fns\n", total_seconds * 1e9);
    } else if (total_seconds < 1e-3) {
        printf("Time: %.3fµs\n", total_seconds * 1e6);
    } else if (total_seconds < 1) {
        printf("Time: %.3fms\n", total_seconds * 1e3);
    } else if (total_seconds < 60) {
        printf("Time: %.3fs\n", total_seconds);
    } else {
        uint64_t minutes = (uint64_t)total_seconds / 60;
        double seconds = total_seconds - (minutes * 60);
        printf("Time: %lum %.3fs\n", minutes, seconds);
    }
}

int main(void) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t ans1 = part1();
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Part 1: %lu\n", ans1);
    print_duration(start, end);

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t ans2 = part2();
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Part 2: %lu\n", ans2);
    print_duration(start, end);

    return 0;
}

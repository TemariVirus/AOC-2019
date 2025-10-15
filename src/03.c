#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <hashmap.h>
#include <wyhash.h>

char input_path[] = "inputs/03.txt";

struct Tile {
    int32_t x;
    int32_t y;
};

typedef HASHMAP(struct Tile, int32_t) TileMap;
size_t TileMap_hash(const struct Tile* val) { return wyhash(val, sizeof(struct Tile), 0, _wyp); }
int TileMap_compare(const struct Tile* a, const struct Tile* b) {
    return a->x != b->x || a->y != b->y;
}

void readWire(FILE* fp, TileMap* map) {
    int32_t x = 0, y = 0, step = 0;
    int32_t dx, dy;
    while (true) {
        int dir = fgetc(fp);
        if (dir == 'R') {
            dx = 1;
            dy = 0;
        } else if (dir == 'L') {
            dx = -1;
            dy = 0;
        } else if (dir == 'U') {
            dx = 0;
            dy = 1;
        } else if (dir == 'D') {
            dx = 0;
            dy = -1;
        }

        int32_t dist;
        fscanf(fp, "%d", &dist);
        for (int32_t i = 0; i < dist; i++) {
            x += dx;
            y += dy;
            step++;
            // Screw memory safety and memory leaks
            struct Tile* key = malloc(sizeof(struct Tile));
            *key = (struct Tile){.x = x, .y = y};
            int32_t* val = malloc(sizeof(step));
            *val = step;
            hashmap_put(map, key, val);
        }

        if (fgetc(fp) == '\n' || feof(fp)) {
            break;
        }
    }
}

int64_t part1(void) {
    FILE* fp = fopen(input_path, "r");
    TileMap wire1, wire2;
    hashmap_init(&wire1, TileMap_hash, TileMap_compare);
    hashmap_init(&wire2, TileMap_hash, TileMap_compare);
    readWire(fp, &wire1);
    readWire(fp, &wire2);
    fclose(fp);

    int64_t min_dist = INT64_MAX;
    const struct Tile* key;
    int32_t* val1;
    hashmap_foreach(key, val1, &wire1) {
        int32_t* val2 = hashmap_get(&wire2, key);
        if (!val2) {
            continue;
        }

        int64_t dist = abs(key->x) + abs(key->y);
        if (dist < min_dist) {
            min_dist = dist;
        }
    }

    hashmap_cleanup(&wire2);
    hashmap_cleanup(&wire1);
    return min_dist;
}

int64_t part2(void) {
    FILE* fp = fopen(input_path, "r");
    TileMap wire1, wire2;
    hashmap_init(&wire1, TileMap_hash, TileMap_compare);
    hashmap_init(&wire2, TileMap_hash, TileMap_compare);
    readWire(fp, &wire1);
    readWire(fp, &wire2);
    fclose(fp);

    int64_t min_steps = INT64_MAX;
    const struct Tile* key;
    int32_t* val1;
    hashmap_foreach(key, val1, &wire1) {
        int32_t* val2 = hashmap_get(&wire2, key);
        if (!val2) {
            continue;
        }

        int64_t steps = *val1 + *val2;
        if (steps < min_steps) {
            min_steps = steps;
        }
    }

    hashmap_cleanup(&wire2);
    hashmap_cleanup(&wire1);
    return min_steps;
}

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <hashmap.h>

#define TYPE char*
#define TYPED(THING) s_##THING
#include "list.c"

extern char input_path[];

typedef struct {
    char* parent;
    s_list children;
} Orbit;
typedef HASHMAP(char, Orbit) OrbitTree;
typedef HASHMAP(char, void) OrbitSet;
int OrbitTree_compare(const char* a, const char* b) { return strcmp(a, b); }

Orbit* OrbitTree_get_or_create(OrbitTree* tree, char* label) {
    Orbit* orbit = hashmap_get(tree, label);
    if (!orbit) {
        orbit = malloc(sizeof(Orbit));
        *orbit = (Orbit){NULL, s_list_init()};
        hashmap_put(tree, label, orbit);
    }
    return orbit;
}

OrbitTree parse_input(void) {
    FILE* fp = fopen(input_path, "r");
    OrbitTree tree;
    hashmap_init(&tree, hashmap_hash_string, OrbitTree_compare);

    while (true) {
        char* label1 = NULL;
        size_t size;
        ssize_t len = getdelim(&label1, &size, ')', fp);
        if (len == -1) {
            break;
        }
        label1[len - 1] = '\0'; // Remove delimiter

        char* label2 = NULL;
        len = getdelim(&label2, &size, '\n', fp);
        label2[len - 1] = '\0'; // Remove delimiter

        Orbit* orbit = OrbitTree_get_or_create(&tree, label1);
        s_list_append(&orbit->children, label2);

        orbit = OrbitTree_get_or_create(&tree, label2);
        orbit->parent = label1;
    }

    fclose(fp);
    return tree;
}

int64_t part1(void) {
    OrbitTree tree = parse_input();

    int64_t count = 0, level = 0;
    s_list to_visit = s_list_init();
    s_list to_visit_next = s_list_init();
    s_list_append(&to_visit, "COM");
    while (true) {
        while (to_visit.length > 0) {
            char* parent = s_list_pop(&to_visit);
            count += level;

            Orbit* orbit = hashmap_get(&tree, parent);
            for (size_t i = 0; i < orbit->children.length; i++) {
                s_list_append(&to_visit_next, s_list_get(&orbit->children, i));
            }
        }
        if (to_visit_next.length == 0) {
            break;
        }

        s_list tmp = to_visit;
        to_visit = to_visit_next;
        to_visit_next = tmp;
        s_list_clear(&to_visit_next);
        level++;
    }

    s_list_free(&to_visit);
    s_list_free(&to_visit_next);
    return count;
}

bool step_bfs_boundary(OrbitTree* tree, s_list* to_visit, s_list* to_visit_next, OrbitSet* visited,
                       const OrbitSet* other) {
    s_list_clear(to_visit_next);
    while (to_visit->length > 0) {
        char* current = s_list_pop(to_visit);
        hashmap_put(visited, current, (void*)1);
        if (hashmap_contains(other, current)) {
            return true;
        }

        Orbit* orbit = hashmap_get(tree, current);
        if (orbit->parent && !hashmap_contains(visited, orbit->parent)) {
            s_list_append(to_visit_next, orbit->parent);
        }
        for (size_t i = 0; i < orbit->children.length; i++) {
            char* child = s_list_get(&orbit->children, i);
            if (!hashmap_contains(visited, child)) {
                s_list_append(to_visit_next, child);
            }
        }
    }
    return false;
}

int64_t part2(void) {
    OrbitTree tree = parse_input();
    char* start = hashmap_get(&tree, "YOU")->parent;
    char* end = hashmap_get(&tree, "SAN")->parent;

    // Meet in the middle breadth-first search
    OrbitSet visited1, visited2;
    hashmap_init(&visited1, hashmap_hash_string, OrbitTree_compare);
    hashmap_init(&visited2, hashmap_hash_string, OrbitTree_compare);
    int64_t dist = -1;
    s_list to_visit1 = s_list_init(), to_visit2 = s_list_init();
    s_list to_visit_next = s_list_init();
    s_list_append(&to_visit1, start);
    s_list_append(&to_visit2, end);
    while (true) {
        if (step_bfs_boundary(&tree, &to_visit1, &to_visit_next, &visited1, &visited2)) {
            break;
        }
        s_list tmp = to_visit1;
        to_visit1 = to_visit_next;
        to_visit_next = tmp;
        dist++;

        if (step_bfs_boundary(&tree, &to_visit2, &to_visit_next, &visited2, &visited1)) {
            break;
        }
        tmp = to_visit2;
        to_visit2 = to_visit_next;
        to_visit_next = tmp;
        dist++;
    }

    s_list_free(&to_visit_next);
    s_list_free(&to_visit2);
    s_list_free(&to_visit1);
    hashmap_cleanup(&visited2);
    hashmap_cleanup(&visited1);
    return dist;
}

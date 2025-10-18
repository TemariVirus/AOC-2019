#include <stdlib.h>
#include <string.h>

#ifdef TYPE

#ifndef TYPED
#define TYPED(THING) THING
#endif

struct TYPED(list) {
    TYPE* items;
    size_t length;
    size_t capacity;
};

struct TYPED(list) TYPED(list_init)(void) {
    return (struct TYPED(list)){
        .items = NULL,
        .length = 0,
        .capacity = 0,
    };
}

void TYPED(list_free)(struct TYPED(list) * self) {
    if (self->items) {
        free(self->items);
        self->items = NULL;
    }
    self->length = 0;
    self->capacity = 0;
}

TYPE TYPED(list_get)(struct TYPED(list) * self, size_t index) {
    if (index >= self->length) {
        printf("Index out of bounds: %zu >= %zu\n", index, self->length);
        exit(1);
    }
    return self->items[index];
}

void TYPED(list_set)(struct TYPED(list) * self, size_t index, TYPE value) {
    if (index >= self->length) {
        printf("Index out of bounds: %zu >= %zu\n", index, self->length);
        exit(1);
    }
    self->items[index] = value;
}

void TYPED(list_append)(struct TYPED(list) * self, TYPE item) {
    if (self->length >= self->capacity) {
        self->capacity = self->capacity ? self->capacity * 2 : 4;
        self->items = realloc(self->items, self->capacity * sizeof(TYPE));
    }
    self->items[self->length++] = item;
}

TYPE TYPED(list_remove)(struct TYPED(list) * self, size_t index) {
    if (index >= self->length) {
        printf("Index out of bounds: %zu >= %zu\n", index, self->length);
        exit(1);
    }
    TYPE item = self->items[index];
    memmove(&self->items[index], &self->items[index + 1],
            (self->length - index - 1) * sizeof(TYPE));
    self->length--;
    return item;
}

struct TYPED(list) TYPED(list_clone)(struct TYPED(list) * self) {
    if (!self->items) {
        return TYPED(list_init)();
    }
    TYPE* new_items = malloc(self->length * sizeof(TYPE));
    memcpy(new_items, self->items, self->length * sizeof(TYPE));
    return (struct TYPED(list)){
        .items = new_items,
        .length = self->length,
        .capacity = self->length,
    };
}

#undef TYPE
#undef TYPED

#endif

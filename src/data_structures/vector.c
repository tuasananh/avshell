#include "vector.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 8

void vector_init(Vector* v, size_t element_size) {
  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
  v->element_size = element_size;
}

bool vector_push(Vector* v, const void* element) {
  if (v->size == v->capacity) {
    size_t new_capacity =
        v->capacity == 0 ? VECTOR_INITIAL_CAPACITY : v->capacity * 2;
    void* new_data = realloc(v->data, new_capacity * v->element_size);
    if (new_data == NULL) return false;
    v->data = new_data;
    v->capacity = new_capacity;
  }
  memcpy((char*)v->data + v->size * v->element_size, element, v->element_size);
  v->size++;
  return true;
}

void* vector_get(const Vector* v, size_t index) {
  return (char*)v->data + index * v->element_size;
}

size_t vector_size(const Vector* v) { return v->size; }

void vector_remove(Vector* v, size_t index) {
  if (index >= v->size) return;
  size_t to_move = v->size - index - 1;
  if (to_move > 0) {
    memmove((char*)v->data + index * v->element_size,
            (char*)v->data + (index + 1) * v->element_size,
            to_move * v->element_size);
  }
  v->size--;
}

void vector_free(Vector* v) {
  free(v->data);
  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
}

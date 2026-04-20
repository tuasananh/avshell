#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  void* data;
  size_t size;
  size_t capacity;
  size_t element_size;
} Vector;

void vector_init(Vector* v, size_t element_size);
bool vector_push(Vector* v, const void* element);
void* vector_get(const Vector* v, size_t index);
size_t vector_size(const Vector* v);
void vector_remove(Vector* v, size_t index);
void vector_free(Vector* v);

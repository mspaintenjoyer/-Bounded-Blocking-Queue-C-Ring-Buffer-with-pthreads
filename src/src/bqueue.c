#ifndef BQUEUE_H
#define BQUEUE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bqueue bqueue_t;

bqueue_t* bq_create(size_t capacity, size_t item_size);
void      bq_destroy(bqueue_t* q);

int       bq_push(bqueue_t* q, const void* item);     // blocks if full
int       bq_pop(bqueue_t* q, void* out);             // blocks if empty

int       bq_try_push(bqueue_t* q, const void* item); // non-blocking
int       bq_try_pop(bqueue_t* q, void* out);         // non-blocking

size_t    bq_len(bqueue_t* q);
size_t    bq_capacity(bqueue_t* q);

#ifdef __cplusplus
}
#endif

#endif

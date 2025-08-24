#ifndef BQUEUE_H
#define BQUEUE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bqueue bqueue_t;

// Create a bounded queue holding up to capacity items.
// Each item is a fixed-size blob (item_size bytes).
// Returns NULL on error.
bqueue_t* bq_create(size_t capacity, size_t item_size);

// Destroy the queue and free memory.
// Only call when no threads are using it.
void bq_destroy(bqueue_t* q);

// Push an item (blocking). Copies item_size bytes from item into queue.
// Returns 0 on success, -1 on error.
int bq_push(bqueue_t* q, const void* item);

// Pop an item (blocking). Copies item_size bytes into out.
// Returns 0 on success, -1 on error.
int bq_pop(bqueue_t* q, void* out);

// Non-blocking try-push. Returns 0 if enqueued, 1 if full, -1 on error.
int bq_try_push(bqueue_t* q, const void* item);

// Non-blocking try-pop. Returns 0 if dequeued, 1 if empty, -1 on error.
int bq_try_pop(bqueue_t* q, void* out);

// Current length (approximate under concurrency).
size_t bq_len(bqueue_t* q);

// Capacity
size_t bq_capacity(bqueue_t* q);

#ifdef __cplusplus
}
#endif

#endif // BQUEUE_H

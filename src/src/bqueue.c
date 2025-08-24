#define _GNU_SOURCE
#include "bqueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct bqueue {
    size_t cap, item_size;
    size_t head, tail, len;
    unsigned char* buf;

    pthread_mutex_t mu;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
};

static inline size_t step(size_t i, size_t cap) { return (i + 1) % cap; }

bqueue_t* bq_create(size_t capacity, size_t item_size) {
    if (!capacity || !item_size) return NULL;
    bqueue_t* q = (bqueue_t*)calloc(1, sizeof(*q));
    if (!q) return NULL;
    q->cap = capacity;
    q->item_size = item_size;
    q->buf = (unsigned char*)malloc(capacity * item_size);
    if (!q->buf) { free(q); return NULL; }
    pthread_mutex_init(&q->mu, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
    return q;
}

void bq_destroy(bqueue_t* q) {
    if (!q) return;
    pthread_mutex_destroy(&q->mu);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    free(q->buf);
    free(q);
}

int bq_push(bqueue_t* q, const void* item) {
    if (!q || !item) return -1;
    pthread_mutex_lock(&q->mu);
    while (q->len == q->cap) pthread_cond_wait(&q->not_full, &q->mu);
    memcpy(q->buf + q->tail * q->item_size, item, q->item_size);
    q->tail = step(q->tail, q->cap);
    q->len++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mu);
    return 0;
}

int bq_pop(bqueue_t* q, void* out) {
    if (!q || !out) return -1;
    pthread_mutex_lock(&q->mu);
    while (q->len == 0) pthread_cond_wait(&q->not_empty, &q->mu);
    memcpy(out, q->buf + q->head * q->item_size, q->item_size);
    q->head = step(q->head, q->cap);
    q->len--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mu);
    return 0;
}

int bq_try_push(bqueue_t* q, const void* item) {
    if (!q || !item) return -1;
    int ret = 0;
    pthread_mutex_lock(&q->mu);
    if (q->len == q->cap) {
        ret = 1;
    } else {
        memcpy(q->buf + q->tail * q->item_size, item, q->item_size);
        q->tail = step(q->tail, q->cap);
        q->len++;
        pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->mu);
    return ret;
}

int bq_try_pop(bqueue_t* q, void* out) {
    if (!q || !out) return -1;
    int ret = 0;
    pthread_mutex_lock(&q->mu);
    if (q->len == 0) {
        ret = 1;
    } else {
        memcpy(out, q->buf + q->head * q->item_size, q->item_size);
        q->head = step(q->head, q->cap);
        q->len--;
        pthread_cond_signal(&q->not_full);
    }
    pthread_mutex_unlock(&q->mu);
    return ret;
}

size_t bq_len(bqueue_t* q) {
    if (!q) return 0;
    pthread_mutex_lock(&q->mu);
    size_t n = q->len;
    pthread_mutex_unlock(&q->mu);
    return n;
}

size_t bq_capacity(bqueue_t* q) {
    return q ? q->cap : 0;
}

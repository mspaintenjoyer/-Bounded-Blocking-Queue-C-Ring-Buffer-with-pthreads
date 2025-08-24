#include "include/bqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

typedef struct { int v; } item_t;

typedef struct {
    bqueue_t* q;
    int count;
} ctx_t;

void* producer(void* arg) {
    ctx_t* c = (ctx_t*)arg;
    for (int i = 0; i < c->count; i++) {
        item_t it = { .v = i };
        int r = bq_push(c->q, &it);
        assert(r == 0);
    }
    return NULL;
}

void* consumer(void* arg) {
    ctx_t* c = (ctx_t*)arg;
    for (int i = 0; i < c->count; i++) {
        item_t it;
        int r = bq_pop(c->q, &it);
        assert(r == 0);
        if ((i % 1000) == 0) usleep(1000);
    }
    return NULL;
}

int main(void) {
    bqueue_t* q = bq_create(128, sizeof(item_t));
    if (!q) { fprintf(stderr, "failed to create queue\n"); return 1; }

    ctx_t ctx = { .q = q, .count = 5000 };

    pthread_t pt, ct;
    pthread_create(&pt, NULL, producer, &ctx);
    pthread_create(&ct, NULL, consumer, &ctx);

    pthread_join(pt, NULL);
    pthread_join(ct, NULL);

    printf("len=%zu cap=%zu\n", bq_len(q), bq_capacity(q));
    bq_destroy(q);
    return 0;
}

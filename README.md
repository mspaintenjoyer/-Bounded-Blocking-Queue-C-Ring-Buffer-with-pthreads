# Bounded Blocking Queue (C)

A minimal, thread-safe bounded queue using a circular buffer and POSIX threads. Great for learning producer/consumer patterns and synchronization.

## Problem statement
Implementing a fixed-capacity queue that:
- Stores fixed-size items (blobs)
- Supports blocking `push` and `pop` with a mutex + condition variables
- Provides non-blocking `try_push` and `try_pop`
- Exposes simple stats (`len`, `capacity`)

## Build and run
- cc -O2 -Wall -Wextra -std=c11 -Iinclude src/bqueue.c demo.c -o demo -lpthread
- ./demo


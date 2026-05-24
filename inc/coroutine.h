#ifndef COROUTINE_H
#define COROUTINE_H

//https://gist.github.com/lpereira/2154951
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ucontext.h>

typedef struct coro_t_		coro_t;
typedef struct thread_t_	thread_t;
typedef int (*coro_function_t)(coro_t *coro);
typedef enum {
    CORO_NEW,
    CORO_RUNNING,
    CORO_FINISHED
} coro_state_t;

#ifdef __x86_64__
union ptr_splitter {
    void *ptr;
    uint32_t part[sizeof(void *) / sizeof(uint32_t)];
};
#endif

struct thread_t_ {
    struct {
        ucontext_t callee, caller;
    } coro;
};

struct coro_t_ {
    coro_state_t state;
    coro_function_t function;
    thread_t *thread;

    ucontext_t context;
    char *stack;
    int yield_value;

	void* ctx[4];
};

void coro_yield(coro_t *coro, int value);

coro_t *coro_new(thread_t *thread, coro_function_t function);

int coro_resume(coro_t *coro);

void coro_yield(coro_t *coro, int value);

void coro_free(coro_t *coro);
#endif
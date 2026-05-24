//https://gist.github.com/lpereira/2154951
#include "coroutine.h"

static const int default_stack_size = 4096;

void coro_yield(coro_t *coro, int value);

#ifdef __x86_64__
static void
_coro_entry_point(uint32_t part0, uint32_t part1)
{
    union ptr_splitter p;
    p.part[0] = part0;
    p.part[1] = part1;
    coro_t *coro = p.ptr;
    int return_value = coro->function(coro);
    coro->state = CORO_FINISHED;
    coro_yield(coro, return_value);
}
#else
static void
_coro_entry_point(coro_t *coro)
{
    int return_value = coro->function(coro);
    coro->state = CORO_FINISHED;
    coro_yield(coro, return_value);
}
#endif

coro_t *
coro_new(thread_t *thread, coro_function_t function)
{
    coro_t *coro = calloc(1, sizeof(*coro));

    coro->state = CORO_NEW;
    coro->stack = calloc(1, default_stack_size);
    coro->thread = thread;
    coro->function = function;

    getcontext(&coro->context);
    coro->context.uc_stack.ss_sp = coro->stack;
    coro->context.uc_stack.ss_size = default_stack_size;
    coro->context.uc_link = 0;

#ifdef __x86_64__
    union ptr_splitter p;
    p.ptr = coro;
    makecontext(&coro->context, (void (*)())_coro_entry_point, 2, p.part[0], p.part[1]);
#else
    makecontext(&coro->context, (void (*)())_coro_entry_point, 1, coro);
#endif

    return coro;
}

int
coro_resume(coro_t *coro)
{
    if (coro == NULL)
        return 0;
    if (coro->state == CORO_NEW)
        coro->state = CORO_RUNNING;
    else if (coro->state == CORO_FINISHED)
        return 0;

    ucontext_t old_context = coro->thread->coro.caller;
    swapcontext(&coro->thread->coro.caller, &coro->context);
    coro->context = coro->thread->coro.callee;
    coro->thread->coro.caller = old_context;

    return coro->yield_value;
}

void
coro_yield(coro_t *coro, int value)
{
    if (coro == NULL || coro->state != CORO_RUNNING)
        return;
    coro->yield_value = value;
    swapcontext(&coro->thread->coro.callee, &coro->thread->coro.caller);
}

void
coro_free(coro_t *coro)
{
    if (coro == NULL)
        return;
    free(coro->stack);
    free(coro);
}
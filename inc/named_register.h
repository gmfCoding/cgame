#ifndef NAMED_REGISTER_H
#define NAMED_REGISTER_H
#include <stdbool.h>

typedef struct s_engine t_engine;

typedef struct s_named_register
{
    char name[64];
    void *ptr;
    bool used;
} t_named_register;


void** named_register_get(t_engine* engine, const char *name);
t_named_register* named_register_create(t_engine* engine, const char *name);
#endif
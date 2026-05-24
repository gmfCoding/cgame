#include "named_register.h"
#include "engine.h"

t_named_register* named_register_create(t_engine* engine, const char *name)
{
    t_named_register reg;
    strncpy(reg.name, name, 64);

    for (int i = 0; i < 100; i++)
    {
        if (!engine->named_registers[i].used)
        {
            engine->named_registers[i] = reg;
            engine->named_registers[i].used = true;
            return &engine->named_registers[i];
        }
    }   
    assert(false && "No more slots available for a named register");
}

void** named_register_get(t_engine* engine, const char *name)
{
    for (int i = 0; i < 100; i++)
    {
        if (engine->named_registers[i].used && strcmp(engine->named_registers[i].name, name) == 0)
        {
            return &engine->named_registers[i].ptr;
        }
    }
    return NULL;
}
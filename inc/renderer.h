#ifndef RENDERER_H 
#define RENDERER_H

#include "cont/set_entptr.h"

#define i_type set_entptr
#define i_key t_entity*
#define i_header
#include "stc/hset.h"

typedef struct
{
    set_entptr entities;

} t_render_ctx;
#endif
/*
 * @Author: christopher.lovell 
 * @Date: 2024-07-01 18:47:59 
 * @Last Modified by: christopher.lovell
 * @Last Modified time: 2024-07-01 19:11:22
 */
#ifndef ASSET_H
# define ASSET_H
#include "io.h"

typedef enum
{
    ASSET_NOT_FOUND,
    ASSET_GLTF_MODEL,
    ASSET_TEXTURE,
    ASSET_TEXT,
    ASSET_CONFIG,
    ASSET_BINARY,
    ASSET_VERT_SHADER,
    ASSET_FRAG_SHADER,
} t_assettype;

typedef struct
{
    t_assettype type;
    char        *path;
    char        *name;
} t_asset;

typedef struct
{
    t_asset base;
    char    *data;
    /* data */
} t_asset_text;

#define ASSET_GETPATH_COUNT 10
char *asset_get_path(char current[PATH_MAX], size_t argc, ...);
t_asset* asset_load(const char *const path, t_assettype type);

#endif
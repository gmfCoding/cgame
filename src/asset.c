/*
 * @Author: christopher.lovell 
 * @Date: 2024-07-01 18:56:42 
 * @Last Modified by: christopher.lovell
 * @Last Modified time: 2024-07-01 19:40:07
 */
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "libcml.h"
#include "io.h"
#include "asset.h"

static const char *assettypename_map[] =
{
    [ASSET_GLTF_MODEL] = "ASSET_GLTF_MODEL",  
    [ASSET_TEXTURE] = "ASSET_TEXTURE",  
    [ASSET_TEXT] = "ASSET_TEXT",  
    [ASSET_CONFIG] = "ASSET_CONFIG",  
    [ASSET_BINARY] = "ASSET_BINARY",  
};

static const size_t assetsize_map[] =
{
    [ASSET_GLTF_MODEL] = sizeof(t_asset),  
    [ASSET_TEXTURE] = sizeof(t_asset),  
    [ASSET_TEXT] = sizeof(t_asset_text),  
    [ASSET_CONFIG] = sizeof(t_asset),  
    [ASSET_BINARY] = sizeof(t_asset),  
};

t_assettype asset_getassettype(const char *const type);
t_asset* asset_load(const char *const path, t_assettype type);

t_assettype asset_getassettype(const char *const type)
{
    for (size_t i = 0; i < sizeof(assettypename_map) / sizeof(char *); i++)
    {
        if (strcmp(assettypename_map[i], type) == 0)
            return (i);
    }
    return ASSET_NOT_FOUND;
}

char *asset_get_path(char current[PATH_MAX], size_t argc, ...)
{
    va_list list;

    va_start(list, argc);
    char *exec_dir = io_get_executable_dir();

    current[0] = '\0';
    cl_strlcat(current, exec_dir, PATH_MAX);
    free(exec_dir);
    io_merge_path_curr(current, "assets");

    for (size_t i = 0; i < argc; i++)
        io_merge_path_curr(current, va_arg(list, char *));
    return current;
}

t_asset* asset_load(const char *const path, t_assettype type)
{
    char asset_path[PATH_MAX] = "assets";
    cl_strlcat(asset_path, path, PATH_MAX);
    t_asset *asset = malloc(assetsize_map[type]);
    char* file = io_fileread(path);
    switch (type)
    {
        case ASSET_TEXT:
        {
            t_asset_text *text = (t_asset_text*)asset;
            text->data = file;
        }
        default:
            break;
    }
    return asset;
}
#include "stb_image.h"
#include "gpu/gpu_texture.h"
#include "logging.h"
#include "util.h"
#include "gldebug.h"

// typedef struct t_resource
// {
// 	const char *name;
// 	t_resource_manager *manager;
// 	void *data;
// } t_resource;

// #include "cont/map_resource.h"

// typedef void (*fn_resource_free)(void *resource);
// typedef void (*fn_resource_add)(t_resource_manager *man, void *resource);

// typedef struct t_resource_manager
// {
// 	map_resource resources;
// 	fn_resource_free free_func;
// 	fn_resource_add add_func;
// } t_resource_manager;

// t_resource *resource_manager_resource_add(t_resource resource)
// {
// 	resource.manager->add_func(resource.manager, resource.data);
// }

void gpu_texture_create(t_gpu_texture* gpu_tex, int width, int height, int colour)
{
	GLCall(glGenTextures(1, &gpu_tex->id));
	GLCall(glBindTexture(GL_TEXTURE_2D, gpu_tex->id));
	// set the texture wrapping/filtering options (on the currently bound texture object)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	int* data = malloc(width * height * sizeof(int)); 
	for (int i = 0; i < width * height; i++)
	{
		data[i] = colour; // Red with full alpha
	}
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	free(data);
}

void gpu_texture_add(t_gpu_texture* gpu_tex, const char *name, const char *asset_path, int channels)
{
	UNUSED(name);
	GLCall(glGenTextures(1, &gpu_tex->id));
	te_logf(LOG_LEVEL_INFO, "gpu_texture", "Generated Texture: %u\n", gpu_tex->id);
	GLCall(glBindTexture(GL_TEXTURE_2D, gpu_tex->id));
	// set the texture wrapping/filtering options (on the currently bound texture object)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_set_flip_vertically_on_load(true);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(asset_path, &width, &height, &nrChannels, channels);
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		te_logf(LOG_LEVEL_ERROR, "gpu_texture", "Failed to load texture at path: %s\n", asset_path);
	}
	stbi_image_free(data);
}
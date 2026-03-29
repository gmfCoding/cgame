#include "material_system.h"
#include "io.h"
#include "gpu/gpu_texture.h"

void materials_setup(t_material_system *system)
{
	char path[PATH_MAX];
	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_colour_vertex.glsl");
		char *basic_vertex = strdup(path);
		
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_colour_fragment.glsl");
		char *basic_fragment = strdup(path);
		
		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_colour_shader"});	
		t_material *basic = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_colour_material"});

		// vec_mat_prop props = {};
		// vec_mat_prop_push(&props, (t_mat_prop){.name = "MVP", .type=MPT_MAT4});
		material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(basic, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});

		free(basic_fragment);
		free(basic_vertex);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_fragment.glsl");
		char *basic_fragment = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_geo.glsl");
		char *basic_geo = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag_geo(basic_vertex, basic_fragment,basic_geo, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_lit_norm_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_lit_norm_material"});
		
		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "uv.png"), 4);

		material_prop_add_new(lit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});	

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "lightColour", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "lightPos", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		material_prop_add_new(lit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(basic_fragment);
		free(basic_vertex);
		free(basic_geo);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "normal_line_vertex.glsl");
		char *vert = strdup(path);

		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "normal_line_fragment.glsl");
		char *frag = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(vert, frag, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "normal_line_shader"});
		t_material *mat = material_system_mat_create(system, (t_material){.shader = shader, .name = "normal_line_material"});

		material_prop_add_new(mat, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(mat, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3 = {0, 1, 1}});

		free(vert);
		free(frag);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_fragment.glsl");
		char *basic_fragment = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_lit_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_lit_material"});
		
		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "uv.png"), 4);

		material_prop_add_new(lit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});	

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "lightColour", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "lightPos", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		material_prop_add_new(lit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(basic_fragment);
		free(basic_vertex);
	}
}
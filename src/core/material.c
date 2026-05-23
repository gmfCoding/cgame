#include "material.h"
#include "load_glad.h"
#include "shader.h"
#include "renderer.h"
#include "logging.h"
#include "gldebug.h"

const char* e_mat_prop_type_name[] =
{
    [MPT_BOOL] = "MPT_BOOL",
    [MPT_INT1] = "MPT_INT1",
    [MPT_INT2] = "MPT_INT2",
    [MPT_INT3] = "MPT_INT3",
    [MPT_INT4] = "MPT_INT4",
    [MPT_UINT1] = "MPT_UINT1",
    [MPT_UINT2] = "MPT_UINT2",
    [MPT_UINT3] = "MPT_UINT3",
    [MPT_UINT4] = "MPT_UINT4",
    [MPT_FLOAT1] = "MPT_FLOAT1",
    [MPT_FLOAT2] = "MPT_FLOAT2",
    [MPT_FLOAT3] = "MPT_FLOAT3",
    [MPT_FLOAT4] = "MPT_FLOAT4",
    [MPT_MAT4] = "MPT_MAT4",
    [MPT_SAMPLER2D] = "MPT_SAMPLER2D",
};


t_mat_prop *material_prop_get(t_material *mat, const char* name)
{
	if (mat == NULL || name == NULL)
		return NULL;
	if (map_mat_prop_contains(&mat->properties, name) == false)
		return NULL;
	map_mat_prop_value *a = map_mat_prop_get_mut(&mat->properties, name);
	if (a == NULL)
		return te_logf(LOG_LEVEL_ERROR, "material", "Material '%s' does not contain property '%s'", mat->name, name), NULL;
    t_mat_prop *prop = &a->second;
    return prop;
}

bool mat_prop_location_try_load(t_material *mat, t_mat_prop *prop)
{	
	if (prop == NULL || mat == NULL || mat->shader == NULL)
		return false;
	if (prop->location_status == MPLS_UNLOADED)
	{
		GLCall_Extra(glGetUniformLocation(mat->shader->shader_id, prop->name));
		prop->location = glGetUniformLocation(mat->shader->shader_id, prop->name);
		if (prop->location < 0)
		{
			te_logf(LOG_LEVEL_WARNING, "material", "Uniform '%s' does not exist in shader '%s' for material '%s'", prop->name, mat->shader->name, mat->name);
			prop->location_status = MPLS_NONEXIST;
			return false;
		}
		prop->location_status = MPLS_LOADED;
	}
	return true;
}

void mat_prop_type_sprintf(char* dest, size_t s, t_mat_prop_type type, t_mat_prop_value value)
{
	size_t max = s;

	switch (type)
	{
		case MPT_BOOL: snprintf(dest, s, "%u", value.u1); break;
		case MPT_INT1: snprintf(dest, s, "%d", value.i1); break;
		case MPT_INT2: snprintf(dest, s, "(%d, %d)", value.i2[0], value.i2[1]); break;
		case MPT_INT3: snprintf(dest, s, "(%d, %d, %d)", value.i3[0], value.i3[1],  value.i3[2]); break;
		case MPT_INT4: snprintf(dest, s, "(%d, %d, %d, %d)", value.i4[0], value.i4[1], value.i4[2], value.i4[3]); break;

		case MPT_UINT1: snprintf(dest, s, "%u", value.u1); break;
		case MPT_UINT2: snprintf(dest, s, "(%u, %u)", value.u2[0], value.u2[1]); break;
		case MPT_UINT3: snprintf(dest, s, "(%u, %u, %u)", value.u3[0], value.u3[1], value.u3[2]); break;
		case MPT_UINT4: snprintf(dest, s, "(%u, %u, %u, %u)", value.u4[0], value.u4[1], value.u4[2], value.u4[3]); break;

		case MPT_FLOAT1: snprintf(dest, s, "%f", value.f1); break;
		case MPT_FLOAT2: snprintf(dest, s, "(%f, %f)", value.f2[0], value.f2[1]); break;
		case MPT_FLOAT3: snprintf(dest, s, "(%f, %f, %f)", value.f3[0], value.f3[1], value.f3[2]); break;
		case MPT_FLOAT4: snprintf(dest, s, "(%f, %f, %f, %f)", value.f4[0], value.f4[1], value.f4[2], value.f4[3]); break;
		case MPT_SAMPLER2D:
			snprintf(dest, s, "(.slot = %d, .tex = %d)", value.texslot.tex, value.texslot.slot);
			break;
		case MPT_MAT4:
			s -= snprintf(dest + max - s, s, "(%f, %f, %f, %f", value.mat[0][0], value.mat[1][0], value.mat[2][0], value.mat[3][0]);
			s -= snprintf(dest + max - s, s, "%f, %f, %f, %f", value.mat[0][1], value.mat[1][1], value.mat[2][1], value.mat[3][1]);
			s -= snprintf(dest + max - s, s, "%f, %f, %f, %f", value.mat[0][2], value.mat[1][2], value.mat[2][2], value.mat[3][2]);
			s -= snprintf(dest + max - s, s, "%f, %f, %f, %f)", value.mat[0][3], value.mat[1][3], value.mat[2][3], value.mat[3][3]);
			break;
	default:
		break;
	}
}

void mat_prop_apply(t_material *mat, t_mat_prop *prop)
{
	if (prop == NULL || mat == NULL || mat->shader == NULL)
		return;
	if (!mat_prop_location_try_load(mat, prop))
		return;
	if (prop->location <= -1)
		return;
	// static bool first = false;
	char str[256] = "\0";
	mat_prop_type_sprintf(str, sizeof(str), prop->type, prop->value);
	//te_logf(LOG_LEVEL_INFO, "material", "Applying prop %s [%s]: %s\n", prop->name, e_mat_prop_type_name[prop->type], str);
	switch (prop->type)
	{
		GLCase(MPT_BOOL, u1, glUniform1ui);
		GLCase(MPT_INT1, i1, glUniform1i);
		GLCaseV(MPT_INT2, i2, glUniform2iv);
		GLCaseV(MPT_INT3, i3, glUniform3iv);
		GLCaseV(MPT_INT4, i4, glUniform4iv);

		GLCase(MPT_UINT1, u1, glUniform1ui);
		GLCaseV(MPT_UINT2, u2, glUniform2uiv);
		GLCaseV(MPT_UINT3, u3, glUniform3uiv);
		GLCaseV(MPT_UINT4, u4, glUniform4uiv);

		GLCase(MPT_FLOAT1, f1, glUniform1f);
		GLCaseV(MPT_FLOAT2, f2, glUniform2fv);
		GLCaseV(MPT_FLOAT3, f3, glUniform3fv);
		GLCaseV(MPT_FLOAT4, f4, glUniform4fv);
		case MPT_SAMPLER2D:
			// if (!first)
			// {
			// 	printf("Binding texture %d to slot %d\n", prop->value.texslot.tex, prop->value.texslot.slot);
			// 	first = true;
			// }
			te_logf(LOG_LEVEL_INFO, "glUniform", "Uniform '%s' of type '%s' with value: %s", prop->name, e_mat_prop_type_name[prop->type], str); \

		
			GLCall(glActiveTexture(prop->value.texslot.slot + GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_2D, prop->value.texslot.tex));

			GLCall(glUniform1i(prop->location, prop->value.texslot.slot)); // Assuming slot is GL_TEXTURE0, GL_TEXTURE1, etc.
			break;
		case MPT_MAT4:
			te_logf(LOG_LEVEL_INFO, "glUniform", "Uniform '%s' of type '%s' with value: %s", prop->name, e_mat_prop_type_name[prop->type], str); \

			GLCall(glUniformMatrix4fv(prop->location, 1, GL_FALSE, &prop->value.mat[0][0]));
			break;
	default:
		break;
	}
}

void material_prop_update(t_material *material, t_mat_prop *prop)
{
	material->dirty = true;
	if (prop->mode == MP_MODE_CLEAN)
		prop->mode = MP_MODE_DIRTY;
}

void material_apply(t_render_ctx* context, t_material *mat)
{
	if (mat->shader == NULL)
		return;
	GLCall(glUseProgram(mat->shader->shader_id));
	context->active_shader  = mat->shader;
	if (mat->dirty == false)
		return;
	mat->dirty = false;
	for (set_str_iter it = set_str_begin(&mat->prop_names); it.ref; set_str_next(&it))
	{
		t_mat_prop *prop = material_prop_get(mat, cstr_str(it.ref));
		if (prop == NULL || (prop->mode == MP_MODE_CLEAN && prop->type != MPT_SAMPLER2D))
			continue;
		if (prop->mode == MP_MODE_CLEAN)
			prop->mode = MP_MODE_DIRTY;
		mat_prop_apply(mat, prop);
	}
}

t_mat_prop* material_prop_add(t_material *mat, t_mat_prop *const prop)
{
    map_mat_prop_emplace_or_assign(&mat->properties, prop->name, *prop);
    set_str_emplace(&mat->prop_names, prop->name);
	t_mat_prop* added = &map_mat_prop_get_mut(&mat->properties, prop->name)->second;
	mat_prop_location_try_load(mat, added);
	material_prop_update(mat, added);
	return added;
}

t_mat_prop* material_prop_add_new(t_material *mat, const char*name, t_mat_prop_type type, t_mat_prop_value value)
{
	return material_prop_add(mat, &(t_mat_prop){.name=name, .type=type, .value=value});
}

void material_prop_add_range(t_material *mat, vec_mat_prop properties)
{
    c_foreach(it, vec_mat_prop, properties)
    {  
        material_prop_add(mat, it.ref);
    }
}

void material_prop_update_named(t_material *material, const char* name, t_mat_prop_value value)
{
	t_mat_prop* prop = material_prop_get(material, name);
	if (prop == NULL)
		return te_logf(LOG_LEVEL_WARNING, "material", "Material '%s' does not contain property '%s'", material->name, name);
	prop->value = value;
	material_prop_update(material, prop);
}
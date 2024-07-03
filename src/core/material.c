#include "material.h"

t_mat_prop *material_prop_get(const t_material *mat, const char* name)
{
    t_mat_prop *prop = map_mat_prop_get(&mat->properties, name);
    return prop;
}

void mat_prop_apply(t_mat_prop *prop)
{
	if (prop->location <= -1)
		return;
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
		case MPT_MAT4:
            glUniformMatrix4fv(prop->location, 1, GL_FALSE, &prop->value.mat[0][0]);
			break;
	default:
		break;
	}
}

void material_prop_update(t_material *material, t_mat_prop *prop)
{
    material->dirty = true;
    prop->dirty = true;
}


void material_apply(t_material *mat)
{
    if (mat->dirty == false)
        return;
    mat->dirty = false;
    c_foreach(it, set_str, mat->prop_names)
    {
        t_mat_prop *prop = material_prop_get(mat, *it.ref);
        if (prop->dirty == false)
            continue;
        prop->dirty = false;
        if (prop != NULL)
            mat_prop_apply(prop);
    }
}

void material_prop_add(t_material *mat, t_mat_prop *const prop)
{
    prop->location = glGetUniformLocation(mat->program, prop->name);
    if (prop->location < 0)
        return;
    map_mat_prop_insert_or_assign(&mat->properties, prop->name, *prop);
}

void material_prop_add_range(t_material *mat, vec_mat_prop properties)

{
    c_foreach(it, vec_mat_prop, properties)
    {  
        material_prop_add(mat, it.ref);
    }
}

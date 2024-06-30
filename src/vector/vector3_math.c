/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:49:54 by clovell           #+#    #+#             */
/*   Updated: 2024/01/04 01:17:30 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector3.h"

t_vec3	v3add(t_vec3 f, t_vec3 s)
{
	t_vec3	vec;

	vec.x = f.x + s.x;
	vec.y = f.y + s.y;
	vec.z = f.z + s.z;
	return (vec);
}

t_vec3	v3sub(t_vec3 f, t_vec3 s)
{
	t_vec3	vec;

	vec.x = f.x - s.x;
	vec.y = f.y - s.y;
	vec.z = f.z - s.z;
	return (vec);
}

t_vec3	v3rev(t_vec3 f)
{
	t_vec3	vec;

	vec.x = -f.x;
	vec.y = -f.y;
	vec.z = -f.z;
	return (vec);
}

t_vec3	v3mulv(t_vec3 f, t_vec3 s)
{
	t_vec3	vec;

	vec.x = f.x * s.x;
	vec.y = f.y * s.y;
	vec.z = f.z * s.z;
	return (vec);
}

t_vec3	v3muls(t_vec3 f, t_vecd s)
{
	t_vec3	vec;

	vec.x = f.x * s;
	vec.y = f.y * s;
	vec.z = f.z * s;
	return (vec);
}

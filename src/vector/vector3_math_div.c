/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_math_div.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmordaun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:05:33 by kmordaun          #+#    #+#             */
/*   Updated: 2024/05/03 17:05:54 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector3.h"

t_vec3	v3divs(t_vec3 f, t_vecd s)
{
	t_vec3	vec;

	vec.x = f.x / s;
	vec.y = f.y / s;
	vec.z = f.z / s;
	return (vec);
}

t_vec3	v3sdiv(t_vecd s, t_vec3 f)
{
	t_vec3	vec;

	vec.x = s / f.x;
	vec.y = s / f.y;
	vec.z = s / f.z;
	return (vec);
}

t_vec3	v3inv(t_vec3 f)
{
	t_vec3	vec;

	vec.x = 1.0 / f.x;
	vec.y = 1.0 / f.y;
	vec.z = 1.0 / f.z;
	return (vec);
}

t_vec3	v3norm(t_vec3 vec)
{
	t_vecd	mag;

	mag = v3mag(vec);
	if (mag == 0)
		return (v3new(0, 0, 0));
	vec.x /= mag;
	vec.y /= mag;
	vec.z /= mag;
	return (vec);
}

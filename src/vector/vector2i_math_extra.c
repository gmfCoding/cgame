/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2i_math_extra.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:49:06 by clovell           #+#    #+#             */
/*   Updated: 2023/11/22 20:47:21 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <math.h>
#include "vector2i.h"

t_veci	v2isqrmag(t_vec2i vec)
{
	return (vec.x * vec.x + vec.y * vec.y);
}

t_veci	v2imag(t_vec2i vec)
{
	return (sqrt(v2isqrmag(vec)));
}

t_vec2i	v2inorm(t_vec2i vec)
{
	t_veci	mag;

	mag = v2imag(vec);
	if (mag == 0)
		return (v2inew(0, 0));
	vec.x /= mag;
	vec.y /= mag;
	return (vec);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:50:56 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 17:09:24 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector2.h"
#include <stdbool.h>
#include <math.h>

t_vec2	v2new(t_vecd x, t_vecd y)
{
	t_vec2	vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

bool	v2eq(t_vec2 a, t_vec2 b, t_vecd threshold)
{
	return (v2sqrmag(v2sub(a, b)) < threshold * threshold);
}

t_vecd	v2sqrmag(t_vec2 vec)
{
	return (vec.x * vec.x + vec.y * vec.y);
}

t_vecd	v2mag(t_vec2 vec)
{
	return (sqrt(v2sqrmag(vec)));
}

t_vec2	v2proj2(t_vec2 a, t_vec2 b, t_vec2 c)
{
	const t_vec2	diff = v2sub(a, b);
	const t_vecd	scale = v2dot(diff, v2sub(c, a)) / v2mag(diff);

	return (v2add(v2muls(v2norm(diff), scale), a));
}

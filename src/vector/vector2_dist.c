/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_dist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:43:45 by clovell           #+#    #+#             */
/*   Updated: 2024/04/29 18:10:04 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector2.h"
#include <math.h>

t_vec2	v2diff(t_vec2 f, t_vec2 s)
{
	t_vec2	vec;

	vec.x = 0;
	vec.y = 0;
	if (f.x > s.x)
		vec.x = f.x - s.x;
	else
		vec.x = s.x - f.x;
	if (f.y > s.y)
		vec.y = f.y - s.y;
	else
		vec.y = s.y - f.y;
	return (vec);
}

t_vecd	v2distm(t_vec2 a, t_vec2 b)
{
	const t_vec2	diff = v2diff(a, b);

	return (diff.x + diff.y);
}

t_vecd	v2dist(t_vec2 a, t_vec2 b)
{
	return (v2mag(v2sub(a, b)));
}

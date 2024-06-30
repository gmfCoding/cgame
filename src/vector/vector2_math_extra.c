/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_math_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 13:50:06 by clovell           #+#    #+#             */
/*   Updated: 2024/04/29 18:09:01 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <math.h>
#include "vector2.h"

t_vecd	f2det(t_vecd x1, t_vecd y1, t_vecd x2, t_vecd y2)
{
	return (x1 * y2 - y1 * x2);
}

t_vecd	v2det(t_vec2 a, t_vec2 b)
{
	return (a.x * b.y - a.y * b.x);
}

t_vecd	v2dot(t_vec2 a, t_vec2 b)
{
	return (a.x * b.x + a.y * b.y);
}

t_vec2	v2proj_line(t_vec2 a, t_vec2 b, t_vec2 c)
{
	const t_vec2	diff = v2sub(a, b);
	const t_vec2	dir = v2norm(v2sub(b, c));
	const t_vecd	scale = v2dot(dir, diff);

	return (v2add(b, v2muls(dir, scale)));
}

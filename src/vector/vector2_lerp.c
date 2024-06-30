/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_lerp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 03:07:14 by clovell           #+#    #+#             */
/*   Updated: 2024/04/29 18:37:47 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector2.h"
#include <math.h>

t_vec2	v2lerp(t_vec2 a, t_vec2 b, double t)
{
	return (v2add(a, v2muls(v2sub(b, a), fmin(1, fmax(0, t)))));
}

double	v2invlerp(t_vec2 a, t_vec2 b, t_vec2 c)
{
	const t_vec2	ac = v2sub(c, a);
	const t_vec2	ab = v2sub(b, a);

	return (v2dot(ac, ab) / v2dot(ab, ab));
}

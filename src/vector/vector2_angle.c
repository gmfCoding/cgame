/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_angle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:47:14 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 17:08:37 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <math.h>
#include "vector2.h"
#include "clmath.h"

/* Gets the angle from vec to positive x-axis, range: `[0, TAU]`*/
double	v2x2ang(t_vec2 vec)
{
	return (M_PI - atan2(vec.y, -vec.x));
}

t_vec2	v2clock(t_vec2 s)
{
	return (v2new(s.y, s.x));
}

t_vec2	v2aclock(t_vec2 s)
{
	return (v2new(-s.y, s.x));
}

float	v2v2ang(t_vec2 a, t_vec2 b)
{
	a = v2norm(a);
	b = v2norm(b);
	return (acosf(v2dot(a, b)));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2i_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:49:54 by clovell           #+#    #+#             */
/*   Updated: 2023/11/22 20:47:20 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector2i.h"

t_vec2i	v2iadd(t_vec2i f, t_vec2i s)
{
	t_vec2i	vec;

	vec.x = f.x + s.x;
	vec.y = f.y + s.y;
	return (vec);
}

t_vec2i	v2isub(t_vec2i f, t_vec2i s)
{
	t_vec2i	vec;

	vec.x = f.x - s.x;
	vec.y = f.y - s.y;
	return (vec);
}

t_vec2i	v2iinv(t_vec2i f)
{
	t_vec2i	vec;

	vec.x = -f.x;
	vec.y = -f.y;
	return (vec);
}

t_vec2i	v2imulv(t_vec2i f, t_vec2i s)
{
	t_vec2i	vec;

	vec.x = f.x * s.x;
	vec.y = f.y * s.y;
	return (vec);
}

t_vec2i	v2imuls(t_vec2i f, t_veci s)
{
	t_vec2i	vec;

	vec.x = f.x * s;
	vec.y = f.y * s;
	return (vec);
}

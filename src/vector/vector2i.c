/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2i.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:50:56 by clovell           #+#    #+#             */
/*   Updated: 2024/04/06 19:27:37 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vector2i.h"
#include <stdbool.h>

t_vec2i	v2inew(t_veci x, t_veci y)
{
	t_vec2i	vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

bool	v2ieq(t_vec2i a, t_vec2i b)
{
	return (a.x == b.x && a.y == b.y);
}

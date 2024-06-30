/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_conv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 10:30:12 by clovell           #+#    #+#             */
/*   Updated: 2024/05/06 21:45:58 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector2.h"
#include "vector3.h"
#include "vector2i.h"

t_vec2	v2itov2(t_vec2i a)
{
	t_vec2	b;

	b.x = a.x;
	b.y = a.y;
	return (b);
}

t_vec2i	v2tov2i(t_vec2 a)
{
	t_vec2i	b;

	b.x = a.x;
	b.y = a.y;
	return (b);
}

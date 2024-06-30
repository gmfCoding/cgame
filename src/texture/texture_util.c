/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:30:29 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:38:11 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include "texture.h"
#include "vector2.h"

#define SET pixel_set
#define GET pixel_get

void	texture_clear(t_texture src, int colour)
{
	int	x;
	int	y;

	y = -1;
	while (++y < src.height)
	{
		x = -1;
		while (++x < src.width)
		{
			pixel_set(src, x, y, colour);
		}
	}
}

t_texture	texture_copy(t_texture tex, \
		const t_vec2 reg[2], int scale)
{
	t_texture	new;
	t_vec2		size;
	t_vec2		pos;
	t_vec2		o;
	int			col;

	o = reg[0];
	size = v2sub(reg[1], reg[0]);
	new = texture_create(size.x * scale, size.y * scale);
	pos.y = 0;
	while (pos.y < size.y * scale)
	{
		pos.x = 0;
		while (pos.x < size.x * scale)
		{
			col = GET(tex, o.x + pos.x / scale, o.y + pos.y / scale);
			SET(new, pos.x, pos.y, col);
			pos.x++;
		}
		pos.y++;
	}
	return (new);
}

void	texture_rotate(t_texture src, int rot)
{
	int			y;
	int			x;
	t_texture	cpy;

	cpy = texture_copy(src, (t_vec2[2]){{.x = 0, .y = 0}, { .x = src.width, .y = src.height }}, 1);
	y = 0;
	while (y < src.height)
	{
		x = 0;
		while (x < src.width)
		{
			if (rot == 1)
				SET(src, x, y, GET(cpy, y, cpy.width - x - 1));
			else if (rot == 2)
				SET(src, x, y, GET(cpy, cpy.width - x - 1, cpy.height - y - 1));
			else if (rot == 3)
				SET(src, x, y, GET(cpy, cpy.height - y - 1, x));
			else
				SET(src, x, y, GET(cpy, x, y));
			x++;
		}
		y++;
	}
	texture_destroy(&cpy, NULL, false);
}

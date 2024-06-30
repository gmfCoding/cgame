/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:48:50 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:01:59 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "texture.h"
#include "vector2i.h"
#include "pixel.h"

static void	draw_circle(const t_texture *tex, t_vec2i pos, t_vec2i quad, \
																	int colour)
{
	t_texture	dst;

	dst = *tex;
	pixel_set_s(dst, pos.x + quad.x, pos.y + quad.y, colour);
	pixel_set_s(dst, pos.x - quad.x, pos.y + quad.y, colour);
	pixel_set_s(dst, pos.x + quad.x, pos.y - quad.y, colour);
	pixel_set_s(dst, pos.x - quad.x, pos.y - quad.y, colour);
	pixel_set_s(dst, pos.x + quad.y, pos.y + quad.x, colour);
	pixel_set_s(dst, pos.x - quad.y, pos.y + quad.x, colour);
	pixel_set_s(dst, pos.x + quad.y, pos.y - quad.x, colour);
	pixel_set_s(dst, pos.x - quad.y, pos.y - quad.x, colour);
}

// Function for circle-generation
// using Bresenham's algorithm
// check for decision parameter
// and correspondingly
// update d, x, y
void	texture_draw_circle(const t_texture *tex, t_vec2i pos, int r, \
																	int colour)
{
	int	x;
	int	d;
	int	y;

	x = 0;
	y = r;
	d = 3 - 2 * r;
	draw_circle(tex, pos, (t_vec2i){.v = {x, y}}, colour);
	while (y >= x)
	{
		x++;
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		draw_circle(tex, pos, (t_vec2i){.v = {x, y}}, colour);
	}
}

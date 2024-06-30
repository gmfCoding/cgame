/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:34:17 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 17:11:17 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "rect.h"
#include "vector2.h"

t_rect	rect_getbounded(t_rect rect)
{
	t_rect	result;

	result.pos = v2sub(rect.pos, v2divs(rect.size, 2));
	result.size = v2add(rect.pos, v2divs(rect.size, 2));
	return (result);
}

bool	rect_contains_v2(t_rect rect, t_vec2 pos)
{
	return (pos.x >= rect.min.x && pos.y >= rect.min.y \
	&& pos.x <= rect.max.x && pos.y <= rect.max.y);
}

t_rect	rect_offset(t_rect rect, t_vec2 offset)
{
	rect.min = v2add(rect.min, offset);
	rect.max = v2add(rect.max, offset);
	return (rect);
}

// // a1 is line1 start, a2 is line1 end, b1 is line2 start, b2 is line2 end
// static bool rect_contains_seg(t_rect rect, t_vec4 line, t_vec2 *out)
// {
//     *out = v2new(0, 0);

//     t_vec2 b = v2sub(line.max, line.min);
//     t_vec2 d = v2sub(line.max, line.min);
//     float bDotDPerp = b.X * d.Y - b.Y * d.X;

//     // if b dot d == 0, it means the lines are parallel so have 
//     infinite intersection points
//     if (bDotDPerp == 0)
//         return false;

//     Vector2 c = b1 - a1;
//     float t = (c.X * d.Y - c.Y * d.X) / bDotDPerp;
//     if (t < 0 || t > 1)
//         return false;

//     float u = (c.X * b.Y - c.Y * b.X) / bDotDPerp;
//     if (u < 0 || u > 1)
//         return false;

//     intersection = a1 + t * b;

//     return true;
// }

static t_vecd	line_side(t_vec2 pos, t_vec4 line)
{
	return ((line.max.y - line.min.y) * pos.x + \
	(line.min.x - line.max.x) * pos.y + v2det(line.max, line.min));
}

bool	rect_contains_seg(t_rect r, t_vec4 line)
{
	const t_vec2	omin = v2new(r.min.x, r.max.y);
	const t_vec2	omax = v2new(r.max.x, r.min.y);
	const int		same = (line_side(r.min, line) > 0) + \
	(line_side(r.max, line) > 0) + (line_side(omin, line) > 0) + \
	(line_side(omax, line) > 0);

	if (same == 0 || same == 4)
		return (false);
	if (line.min.x > r.max.x && line.max.x > r.max.x)
		return (false);
	if (line.min.x < r.min.x && line.max.x < r.min.x)
		return (false);
	if (line.min.y > r.max.y && line.max.y > r.max.y)
		return (false);
	if (line.min.y < r.min.y && line.max.y < r.min.y)
		return (false);
	return (true);
}

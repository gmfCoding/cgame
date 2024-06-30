/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 01:39:55 by clovell           #+#    #+#             */
/*   Updated: 2024/04/20 20:15:46 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WALL_H
# define WALL_H
# include "vector2.h"
# include "vector2i.h"
# include "ray.h"

# define C_WALL 0
# define C_SPRITE 1

typedef struct s_vertical
{
	int			x;
	double		camera_x;
	t_vec2		dir;
	t_rayinfo	ray;

}			t_vert;
/*
typedef union u_tex_quad
{
	// struct s_rect
	struct
	{
		char	n;
		char	s;
		char	w;
		char	e;
	};
	char		v[4];
	int		tex;
} t_tex_quad;
*/
typedef struct s_column
{
//	t_tex_quad	texture;
	int		texture;
	int		tex_size;
	int		shaded;
	double	sample_dy;
	double	uvys;
	double	uvyo;
	int		x;
	int		height;
	t_vec2	uv;
	t_vec2i	sample;
	t_vec2i	range;
	int		type;
}			t_col;

void	render_vertical(t_game *game, t_vert info);
#endif

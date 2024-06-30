/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector4.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:24:08 by clovell           #+#    #+#             */
/*   Updated: 2024/03/19 00:48:40 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VECTOR4_H
# define VECTOR4_H

# include <stdbool.h>
# include "vector2.h"

typedef struct s_vec4	t_vec4;

struct s_vec4
{
	union
	{
		// struct s_rect
		struct
		{
			union
			{
				t_vec2	pos;
				t_vec2	min;
			};
			union
			{
				t_vec2	max;
				t_vec2	size;
			};
		};
		t_vecd		v[4];
		struct
		{
			t_vecd	x;
			t_vecd	y;
			t_vecd	z;
			t_vecd	w;
		};
	};
};

/* Scales the size of a vector by s */
t_vec4	v4muls(t_vec4 f, t_vecd s);
#endif
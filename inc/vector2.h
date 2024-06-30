/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:07:41 by clovell           #+#    #+#             */
/*   Updated: 2024/04/29 18:05:44 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VECTOR2_H
# define VECTOR2_H

typedef double	t_vecd;

typedef struct s_vec2
{
	union
	{
		struct
		{
			union
			{
				t_vecd	x;
				t_vecd	min;
			};
			union
			{
				t_vecd	y;
				t_vecd	max;
			};
		};
		t_vecd	v[2];
	};
}				t_vec2;

t_vec2			v2new(t_vecd x, t_vecd y);

/* Adds a vector to another */
t_vec2			v2add(t_vec2 f, t_vec2 s);

/* Subtracts a vector from another */
t_vec2			v2sub(t_vec2 f, t_vec2 s);

/* Inverts the components of a vector, essentially reversing the direction. */
t_vec2			v2inv(t_vec2 s);

/* Rotates a vector 90 degrees clockwise */
t_vec2			v2clock(t_vec2 s);

/* Rotates a vector 90 degrees anti-clockwise */
t_vec2			v2aclock(t_vec2 s);

/* Reverse the components of a vector, essentially reversing the direction. */
t_vec2			v2rev(t_vec2 f);

/* Multiplies a vector by another */
t_vec2			v2mulv(t_vec2 f, t_vec2 s);

/* Scales the size of a vector by s */
t_vec2			v2muls(t_vec2 f, t_vecd s);

/* Return the squared magnitude of a vector */
t_vecd			v2sqrmag(t_vec2 vec);

/* Returns the magnitude of a vector */
t_vecd			v2mag(t_vec2 vec);

/* Returns a normalised (length of 1) copy of a vector */
t_vec2			v2norm(t_vec2 vec);

/* Same as v2det but args split */
t_vecd			f2det(t_vecd x1, t_vecd y1, t_vecd x2, t_vecd y2);

/* Returns the determinate (perp cross product)*/
t_vecd			v2det(t_vec2 a, t_vec2 b);

/* Returns the dot product. */
t_vecd			v2dot(t_vec2 a, t_vec2 b);

/* Returns component wise division c = c / s */
t_vec2			v2divs(t_vec2 f, t_vecd s);

/* Returns component-wise inverse division c = s / c */
t_vec2			v2sdiv(t_vecd s, t_vec2 f);

/* Returns the interpolation of a to b by t, range: `0 <= t <= 1`*/
t_vec2			v2lerp(t_vec2 a, t_vec2 b, double t);

/* Returns the interpolation factor from a to b at c. (extrapolated) */
double			v2invlerp(t_vec2 a, t_vec2 b, t_vec2 c);

/* Projects A on to the line segment BC. */
t_vec2			v2proj_line(t_vec2 a, t_vec2 b, t_vec2 c);

/* Returns the distance between two vectors. */
t_vecd			v2dist(t_vec2 a, t_vec2 b);

/* Returns the manhatten distance between two vectors a to b. */
t_vecd			v2distm(t_vec2 a, t_vec2 b);

/* Angle between +x acis and vector. */
double			v2x2ang(t_vec2 vec);

/* Angle between vectors a and b. */
float			v2v2ang(t_vec2 a, t_vec2 b);

/* Return the absolute difference between to vectors */
t_vec2			v2diff(t_vec2 f, t_vec2 s);

char			*v2toa(int v[3]);
#endif

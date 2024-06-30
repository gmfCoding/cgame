/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:07:41 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 13:29:03 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VECTOR3_H
# define VECTOR3_H

typedef double	t_vecd;

typedef struct s_vec3
{
	union
	{
		struct
		{
			t_vecd	x;
			t_vecd	y;
			t_vecd	z;
		};
		t_vecd	v[3];
	};
}				t_vec3;

t_vec3	v3new(t_vecd x, t_vecd y, t_vecd z);

/* Adds a vector to another */
t_vec3	v3add(t_vec3 f, t_vec3 s);

/* Subtracts a vector from another */
t_vec3	v3sub(t_vec3 f, t_vec3 s);

/* Reverse the components of a vector, essentially reversing the direction. */
t_vec3	v3rev(t_vec3 s);

/* Multiplies a vector by another */
t_vec3	v3mulv(t_vec3 f, t_vec3 s);

/* Scales the size of a vector by s */
t_vec3	v3muls(t_vec3 f, t_vecd s);

/* Return the squared magnitude of a vector */
t_vecd	v3sqrmag(t_vec3 vec);

/* Returns the magnitude of a vector */
t_vecd	v3mag(t_vec3 vec);

/* Returns a normalised (length of 1) copy of a vector */
t_vec3	v3norm(t_vec3 vec);

/* Returns cross product of two vectors */
t_vec3	v3cross(t_vec3 a, t_vec3 b);

/* Returns dpt product of two vectors */
t_vecd	v3dot(t_vec3 a, t_vec3 b);

/* Returns component wise division c = c / s */
t_vec3	v3divs(t_vec3 f, t_vecd s);

/* Returns component-wise inverse division c = s / c */
t_vec3	v3sdiv(t_vecd s, t_vec3 f);

/* Returns component-wise inverse c = 1 / c */
t_vec3	v3inv(t_vec3 f);

char	*v3toa(double v[3]);

#endif

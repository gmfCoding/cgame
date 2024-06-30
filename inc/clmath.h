/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clmath.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 01:27:13 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:17:33 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CLMATH_H
# define CLMATH_H

# include <stdbool.h>

# ifndef M_PI
#  define M_PI 3.141592653589
# endif
# ifndef M_TAU
#  define M_TAU 6.2831853072
# endif

double	angle_diff(double a, double b);
double	cl_atan2(double x, double y);
double	facos(double x);
double	flerp(double min, double max, double lerp);
double	fclamp(double min, double max, double value);

bool	inside(int x, int y, int max_x, int max_y);
#endif

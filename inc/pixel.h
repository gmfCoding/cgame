/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 03:27:42 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 14:03:09 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIXEL_H
# define PIXEL_H
# include "structs/s_texture.h"
# ifdef __linux__
// Full opaque colour (linux)
#  define R_ALPHA 0xff000000
# else
// Full opaque colour (mac)
#  define R_ALPHA 0x00000000
# endif

// Full red colour
# define R_RED   0x00ff0000
// Full green colour
# define R_GREEN 0x0000ff00
// Full blue colour
# define R_BLUE  0x000000ff

// Red colour mask
# define M_RED   R_RED
// Green colour mask
# define M_GREEN R_GREEN
// Blue colour mask
# define M_BLUE  R_BLUE
// Alpha colour mask
# define M_APLHA 0xff000000
# define M_COL	 0x00ffffff

# define OF_ALPHA 24
# define OF_RED   16
# define OF_GREEN 8
# define OF_BLUE  0

int		colour_blend(int first, int second);
void	pixel_set_s(const t_texture data, const int x, const int y, \
														const int color);
void	pixel_set(const t_texture data, const int x, const int y, \
														const int color);
int		pixel_get_s(const t_texture data, const int x, const int y);
int		pixel_get(const t_texture data, const int x, const int y);
#endif

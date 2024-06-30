/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubed.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmordaun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:31:49 by kmordaun          #+#    #+#             */
/*   Updated: 2024/07/01 03:22:57 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBED_H
# define CUBED_H
# include <inttypes.h>

//#include "extra/optimised.h"
# include "texture.h"
# include "render.h"
# include "map.h"
# include "libcml.h"
# include "get_next_line.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <errno.h>

# define PI 3.141592653589

// define DEG2RAD PI / 180.0
# define DEG2RAD 0.01745329251993888888888888888889
# define RAD2DEG 57.2957795131

# ifdef __linux__
#  define R_ALPHA 0xff000000
# else
#  define R_ALPHA 0x00000000
# endif

# define MAX_RAYCAST_DIST 100
# define MAX_DEPTHS 5
# define MAX_TEX_BF 4

# define OF_ALPHA 24
# define OF_RED   16
# define OF_GREEN 8
# define OF_BLUE  0

typedef struct s_vec2f	t_vec2f;
//typedef struct s_cellinfo t_cellinfo;

typedef uint8_t			t_cellheight;

typedef union u_texid	t_texid;

union u_texid
{
	int8_t : MAX_TEX_BF;
};
/*
struct s_cellinfo
{
	t_cellheight	height;

	t_texid			floor;	
	t_texid			roof;
	t_texid			north;
	t_texid			south;
	t_texid			east;
	t_texid			west;
};

struct s_map
{
	t_cellinfo	cells[MAP_MAX_X * MAP_MAX_Y];
	uint8_t		width;
	uint8_t		height;

};
*/

/* util/time.c */
int64_t	time_get_ms(void);

#endif

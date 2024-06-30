/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 16:11:47 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:25:38 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stb_image.h>
#include <stddef.h>
#include <stdlib.h>
#include "texture.h"
#ifdef __linux__

static void	texture_init(t_texture tex)
{
	int	x;
	int	y;
	int	c;

	y = -1;
	while (++y < tex.height)
	{
		x = -1;
		while (++x < tex.width)
		{
			c = pixel_get(tex, x, y);
			c = ((255 - (c >> OF_ALPHA)) << OF_ALPHA) | (c & 0x00FFFFFF);
			pixel_set(tex, x, y, c);
		}
	}
}
#else

static void	texture_init(t_texture tex)
{
	(void)tex;
}
#endif

t_texture	texture_create(int width, int height)
{
	t_texture	t;

	t.width = width;
	t.height = height;
	t.data = malloc(width * height * sizeof(int));
	t.channel = 4;
	texture_init(t);
	return (t);
}

void	texture_destroy(t_texture *tex, t_texture **tex_ptr, \
	bool use_free)
{
	if (tex == NULL)
		tex = *tex_ptr;
	if (tex == NULL)
		return ;
	if (tex != NULL && tex->img != NULL)
		free(tex->data);
	tex->img = NULL;
	if (tex_ptr != NULL)
		*tex_ptr = NULL;
	if (use_free)
		free(tex);
}

t_texture	texture_load(const char *const path)
{
	t_texture	t;
	t.data = (int *)stbi_load(path, &t.width, &t.height, &t.channel, 4);
	texture_init(t);
	return (t);
}

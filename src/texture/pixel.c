/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:34:56 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:05:03 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "texture.h"

__attribute__((always_inline))
inline void	pixel_set(const t_texture data, const int x, const int y, \
																const int color)
{
	data.data[x + y * data.width] = color;
}

__attribute__((always_inline))
inline void	pixel_set_s(const t_texture data, const int x, const int y, \
																const int color)
{
	if (x < 0 || y < 0 || x >= data.width || y >= data.height)
		return ;
	data.data[x + y * data.width] = color;
}

inline int	pixel_get(const t_texture data, const int x, const int y)
{
	return (data.data[x + y * data.width]);
}

#ifdef D_ASSERT_PIXEL_NO_GET

int	pixel_get_s(t_texture data, int x, int y)
{
	char	*dst;

	if (x < 0 || y < 0 || x > data.width || y > data.height)
		assert(1, ERR_BADPARAM, __FILE__, __LINE__);
	dst = data.data + (y * data.line_size + x * (data.bpp / 8));
	return (*(int *)dst);
}
#else

int	pixel_get_s(const t_texture data, const int x, const int y)
{
	if (x < 0 || y < 0 || x > data.width || y > data.height)
		return (R_ALPHA);
	return (data.data[x + y * data.width]);
}
#endif

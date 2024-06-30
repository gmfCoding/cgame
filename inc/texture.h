/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:13:28 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:38:26 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H
# include "vector2.h"
# include "rect.h"
# include "vector2i.h"
# include "pixel.h"
# include "structs/s_texture.h"

int			colour_blend(int first, int second);
void		pixel_set_s(t_texture data, int x, int y, int color);
void		pixel_set(t_texture data, int x, int y, int color);
int			pixel_get_s(t_texture data, int x, int y);
int			pixel_get(t_texture data, int x, int y);
void		texture_draw_line(t_texture data, t_vec2 start, t_vec2 end, \
																int colour);
void		texture_draw_square(t_texture data, t_vec2 start, t_vec2 width, \
																int colour);
void		texture_draw_circle(const t_texture *tex, t_vec2i pos, int r, \
																int colour);
void		texture_blit_rect(t_texture *dst, t_texture *src, t_rect area);
void		texture_blit_s(t_texture src, t_texture dst, t_vec2 pos, int scale);
void		texture_blit(t_texture src, t_texture dst, t_vec2 pos);
void		texture_clear(t_texture src, int colour);
t_texture	texture_create(int width, int height);
t_texture	texture_copy(t_texture tex, const t_vec2 reg[2], \
																int scale);
/* create a copy of 'tex' that has been rotated by 90-deg * 'rot' */
void		texture_rotate(t_texture tex, int rot);
t_texture	texture_load(const char *const path);
void		texture_destroy(t_texture *tex, \
		t_texture **tex_ptr, bool use_free);

#endif

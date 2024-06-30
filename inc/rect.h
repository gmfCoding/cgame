/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:51:18 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 13:40:22 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef RECT_H
# define RECT_H
# include <stdbool.h>
# include "vector2.h"
# include "vector4.h"

typedef struct s_vec4	t_rect;
t_rect	rect_getbounded(t_rect rect);
t_rect	rect_offset(t_rect rect, t_vec2 offset);
bool	rect_contains_v2(t_rect rect, t_vec2 pos);
bool	rect_contains_seg(t_rect r, t_vec4 line);

#endif

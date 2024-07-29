/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_texture.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 03:32:52 by clovell           #+#    #+#             */
/*   Updated: 2024/07/29 18:08:02 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef S_TEXTURE_H
# define S_TEXTURE_H
typedef union u_colour
{
	int		rgba;
	char	channels[4];
}			t_ucolour;

typedef struct s_texture
{
	int		*data;
	int		width;
	int		height;
	int		channel;
}				t_texture;

#endif
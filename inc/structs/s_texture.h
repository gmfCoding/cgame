/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_texture.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 03:32:52 by clovell           #+#    #+#             */
/*   Updated: 2024/07/05 20:07:33 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef S_TEXTURE_H
# define S_TEXTURE_H

typedef struct s_texture
{
	t_ucolour	*data;
	int			width;
	int			height;
	int			channel;
}				t_texture;

typedef union u_colour
{
	int		rgba;
	char	channels[4];
}			t_ucolour;
#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorconv.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell<marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 01:44:25 by clovell           #+#    #+#             */
/*   Updated: 2024/05/03 13:28:10 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORCONV_H
# define VECTORCONV_H
# include "vector2.h"
# include "vector2i.h"
# include "vector3.h"

t_vec2	v3tov2(t_vec3 a);
t_vec3	v2tov3(t_vec2 a);
t_vec3	v2itov3(t_vec2i a);
t_vec2i	v3tov2i(t_vec3 a);
t_vec2	v2itov2(t_vec2i a);
t_vec2i	v2tov2i(t_vec2 a);
#endif

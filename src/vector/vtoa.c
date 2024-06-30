/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 23:58:09 by clovell           #+#    #+#             */
/*   Updated: 2024/02/05 00:05:48 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"
#include "libcml_int.h"

char	*v3toa(double v[3])
{
	return (cl_strfmt("%f, %f, %f", v[0], v[1], v[2]));
}

char	*v2toa(double v[3])
{
	return (cl_strfmt("%f, %f, %f", v[0], v[1]));
}

// char	*v3itoa(int v[3])
// {
// 	return (cl_strfmt("%i, %i, %i", v[0], v[1], v[2]));
// }

char	*v2itoa(int v[3])
{
	return (cl_strfmt("%i, %i, %i", v[0], v[1]));
}

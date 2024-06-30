/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 14:07:37 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:41:54 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <string.h>

#include "libcml.h"

size_t	cl_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = strlen(src);
	while (j < i && j + 1 < dstsize)
	{
		dst[j] = src[j];
		j++;
	}
	if (dstsize > 0)
		dst[j] = '\0';
	return (i);
}

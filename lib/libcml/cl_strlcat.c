/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 13:55:29 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:42:44 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <string.h>
#include "libcml.h"

size_t	cl_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	end;
	size_t	begin;
	size_t	i;

	begin = strlen(dest);
	end = strlen(src);
	i = 0;
	while (i < end && i + begin + 1 < dstsize)
	{
		dest[begin + i] = src[i];
		i++;
	}
	if (dstsize < begin)
		begin = dstsize;
	else
		dest[begin + i] = '\0';
	return (begin + end);
}

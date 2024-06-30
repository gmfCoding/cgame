/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 17:11:49 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:58:46 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"
#include "libcml_int.h"
#include <stdlib.h>
#include <string.h>

char	*cl_strjoin(char const *s1, char const *s2)
{
	size_t	s1len;
	size_t	s2len;
	char	*dst;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1len = strlen(s1);
	s2len = strlen(s2);
	dst = malloc(s1len + s2len + 1);
	if (dst == NULL)
		return (NULL);
	memcpy(dst, s1, s1len);
	cl_strlcpy(dst + s1len, s2, s2len + 1);
	return (dst);
}

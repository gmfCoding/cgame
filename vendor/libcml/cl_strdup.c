/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 15:25:18 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:43:19 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <string.h>
#include <stdlib.h>

#include "libcml.h"
#include "libcml_int.h"


char	*cl_strdup(const char *s1)
{
	int		len;
	char	*s2;

	len = strlen(s1);
	s2 = calloc(len + 1, sizeof(char));
	if (!s2)
		return (NULL);
	cl_strlcpy(s2, s1, len + 1);
	return (s2);
}

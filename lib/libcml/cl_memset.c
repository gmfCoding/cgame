/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 13:18:51 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:39:25 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"
#include "libcml_int.h"

void	*cl_memsetf64(void *b, double c, size_t len)
{
	while (len > 0)
	{
		len--;
		((double *)b)[len] = c;
	}
	return (b);
}

void	*cl_memsetf32(void *b, float c, size_t len)
{
	while (len > 0)
	{
		len--;
		((float *)b)[len] = c;
	}
	return (b);
}

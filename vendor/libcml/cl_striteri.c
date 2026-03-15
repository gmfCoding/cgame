/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:14:24 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:39:25 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"
#include "libcml_int.h"

void	cl_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	idx;

	idx = 0;
	while (s[idx] != '\0')
	{
		f(idx, &s[idx]);
		idx++;
	}
}

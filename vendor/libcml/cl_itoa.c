/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:20:49 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:57:15 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

static void	cl_itoa_rec(char *dst, int size, long long int n)
{
	if (n == 0 || size < 0)
		return ;
	cl_itoa_rec(dst, size - 1, n / 10);
	dst[size] = n % 10 + '0';
}

size_t	cl_intlen(int64_t	nb)
{
	size_t	count;

	count = (nb < 0) + 1;
	nb /= 10;
	while (nb != 0)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

size_t	cl_ltostr(int64_t num, char *str)
{
	const char	min[] = "-9223372036854775808";
	int64_t		n;
	size_t		count;

	n = num;
	count = cl_intlen(n);
	str[count] = '\0';
	if (num == INT64_MIN)
		return (cl_strlcpy(str, min, sizeof(min)));
	else if (num == 0)
	{
		str[0] = '0';
		return (1);
	}
	else if (num < 0)
		n = 0L - num;
	cl_itoa_rec(str + (num < 0), count - 1 - (num < 0), n);
	if (num < 0)
		str[0] = '-';
	return (count);
}

char	*cl_itoa(int n)
{
	int			count;
	char		*str;

	count = cl_intlen(n);
	str = calloc(count + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	cl_ltostr(n, str);
	return (str);
}

char	*cl_ltoa(long long int n)
{
	int				count;
	char			*str;

	count = cl_intlen(n);
	str = calloc(count + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	cl_ltostr(n, str);
	return (str);
}

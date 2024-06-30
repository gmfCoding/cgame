/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 00:47:04 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:01:00 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libcml.h"

static int	cl_abs(int abs)
{
	if (abs < 0)
		return (-abs);
	return (abs);
}

char	*cl_ftoa(double n, int precision)
{
	char		*whole;
	char		*frac;
	const char	*padding = "";

	whole = cl_ltoa((long long int)n);
	n = cl_abs((n - (long long int)n) * (double)precision);
	frac = cl_ltoa(n);
	if (n < 10)
		padding = "00";
	else if (n < 100)
		padding = "0";
	return (cl_strfmt("%S.%s%S", whole, padding, frac));
}

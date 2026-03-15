/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:59:27 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:52:10 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include "libcml.h"
#include "libcml_int.h"

/* Determines the base of a number if def == 0
   Also sets `after` to be the `start` after the base prefix.
 */
static int	determine_base(char *start, char **after, int def)
{
	if ((def == 0 || def == 16) && start[0] == '0' && start[1] == 'x')
	{
		if (after)
			*after = start + 2;
		return (16);
	}
	else if ((def == 0 || def == 8) && start[0] == '0')
		return (8);
	if (def == 0)
		return (10);
	return (def);
}

/* Returns the value of in `c` in `base`.
	if `c` is outside the base then `base` is returned.
*/
static int	cl_baseval(char c, uint32_t base)
{
	const char	*table = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (base > 36 || base == 0)
		return ((base != 0) * -1);
	c = tolower(c);
	if (c < '0')
		return (-1);
	if (c <= '9' && c <= table[base - 1])
		return (c - '0');
	else if (c >= 'a' && c <= table[base - 1])
		return (c - 'a' + 10);
	return (-1);
}

int64_t	cl_strtol(char *start, char **end, uint32_t base)
{
	int64_t		negative;
	int64_t		accum;

	if (start == NULL || base == 1)
		return (0);
	if (end == NULL)
		end = &start;
	*end = start;
	accum = 0;
	negative = 1;
	while (*start && isspace(*start))
		(start)++;
	negative = (*start == '-') * -1 + (*start != '-');
	start += *start == '-' || *start == '+';
	base = determine_base(start, &start, base);
	while (*start && cl_baseval(*start, base) != -1)
	{
		accum = accum * base + cl_baseval(*start, base);
		(start)++;
		*end = start;
	}
	return (accum * negative);
}
/*


int main(void)
{
	char	*test = "123,145,514";
	char	*end;
	int value = cl_strtol(test, &end, 10);
	printf("value:%d:%s", value, end);
}
*/
/*
void	test(char *str, int base)
{
	char *A = NULL;
	long int a = strtol(str, &A, base);
	char *B = NULL;
	long int b = cl_strtol(str, &B, base);
	if (a != b || A != B)
	{
		printf("error(%d):%s\n", base, str);
		if (a != b)
			printf("    %d != %d\n", a, b);
		else
			printf("    %p != %p | %s != %s\n", A, B, A, B);
		b = cl_strtol(str, &B, base);
	}
	else
	{
		printf("%d:%s passed!\n", base, str);
	}
}

int	main()
{
	char	str[9];
	str[0] = '0';
	str[8] = '\0';
	for (int b = 0; b <= 36; b += 1)
	{
		for (size_t i = 0; i < 10000; i++)
		{
			for (size_t j = 1; j < 8;j++)
			{
				str[j] = (rand() % 36);
				if (str[j] >= 10)
					str[j] += 'a' - 10;
				else
					str[j] += '0';
			}
			test(str, b);
		}
	}
	return (0);
}*/
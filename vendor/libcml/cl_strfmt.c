/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_strfmt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 20:54:17 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:51:12 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include "libcml.h"
#include "libcml_int.h"

static int	get_append(char **format, va_list *list, char **tmp)
{
	char	fmt;

	if (**format == '%')
	{
		fmt = (*format)[1];
		if (fmt == '%')
			*tmp = "%";
		else if (fmt == 'd')
			*tmp = cl_itoa(va_arg(*list, int));
		else if (fmt == 'l')
			*tmp = cl_itoa(va_arg(*list, long long int));
		else if (fmt == 'f')
			*tmp = cl_ftoa(va_arg(*list, double), 1000);
		else if (fmt == 's' || fmt == 'S')
			*tmp = va_arg(*list, char *);
		else if (fmt == 'c')
			*tmp[0] = va_arg(*list, int);
		(*format)++;
		return (strchr("Sldf", fmt) != NULL);
	}
	else
		*tmp[0] = **format;
	return (0);
}

#define BF_BUFFER 0
#define BF_APPEND 1
#define BF_OLDBUF 2

char	*cl_strfmt(char *fmt, ...)
{
	va_list		list;
	char		*buffers[3];
	char		charstr[2];
	int			allocated;

	buffers[BF_BUFFER] = cl_strdup("");
	va_start(list, fmt);
	charstr[1] = '\0';
	while (*fmt)
	{
		buffers[BF_APPEND] = charstr;
		allocated = get_append(&fmt, &list, &buffers[BF_APPEND]);
		buffers[BF_OLDBUF] = buffers[BF_BUFFER];
		buffers[BF_BUFFER] = cl_strjoin(buffers[BF_OLDBUF], buffers[BF_APPEND]);
		free(buffers[BF_OLDBUF]);
		if (allocated)
			free(buffers[BF_APPEND]);
		fmt++;
	}
	va_end(list);
	return (buffers[BF_BUFFER]);
}

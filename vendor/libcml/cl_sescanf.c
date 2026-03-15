/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_sescanf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:57:53 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:52:38 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdbool.h>
#include <ctype.h>
#include <stddef.h>
#include "sescanf_int.h"
#include "libcml.h"
#include "libcml_int.h"

char	sesc_fmt_gettype(char *fmt, int step, char **type_start)
{
	char		*type;

	while (*fmt && step > 0)
	{
		step--;
		fmt++;
	}
	if (step != 0)
	{
		if (type_start)
			*type_start = "\0";
		return ('\0');
	}
	cl_strtol(fmt, &type, 10);
	if (fmt[-1] != '%')
		type = ((char *)"\0\0\0\0\0\0\0\0") + 4;
	if (type_start)
		*type_start = type;
	return (*type);
}

bool	sesc_nodelim_nextis(t_sescanf_ctx *ctx, char type)
{
	return (ctx->fmt[1] != '%' && *ctx->next_type == type);
}

static bool	sesc_handle_nonfmt(t_sescanf_ctx *ctx)
{
	bool	found_isspace;

	found_isspace = false;
	if (*ctx->fmt == '\v')
	{
		while (*ctx->current && isspace(*ctx->current))
		{
			ctx->current++;
			(*ctx->read_ptr)++;
			found_isspace = true;
		}
		if (found_isspace == false && *ctx->current)
			return (true);
	}
	else
	{
		if (*ctx->current != *ctx->fmt)
			return (true);
		(*ctx->read_ptr)++;
		ctx->current++;
	}
	return (false);
}

/*
	SEE: cl_sescanf
*/
int	cl_vsescanf(char *src, char *fmt, va_list *list)
{
	t_sescanf_ctx	ctx;

	ctx = (t_sescanf_ctx){.current = src, .fmt = fmt, .list = list, \
	.source = src, .read_ptr = &ctx.read_total};
	if (src == NULL || fmt == NULL)
		return (0);
	while (*ctx.fmt != '\0')
	{
		if (ctx.fmt[0] == '%' && ctx.fmt[1])
		{
			sesc_fmt_gettype(++ctx.fmt, 0, &ctx.type);
			sesc_fmt_gettype(ctx.type, 2, &ctx.next_type);
			if (sesc_handle_fmt_type(&ctx) == -1)
				return (-ctx.found);
		}
		else
		{
			if (sesc_handle_nonfmt(&ctx))
				return (-ctx.found);
		}
		ctx.fmt++;
	}
	return (ctx.found);
}

/**
 * Scan a string for values and extract.
 * Similar to libc sscanf but with simplified features.
 ```
 // Example
 int d, m, y;
 char *my_data = ""; // Imagine sourcing from an config file instead.
 // It would probably be a good idea to end the format string with slash-v
 cl_sescanf(my_data, "date:%d/%d/%d", &d, &m, &y);
 ```
 * ### Format Specifiers:
  - `%d/i`: `decimal` (`int32_t*`)
  - `%u`: `decimal` (`uint32_t*`)
  - `%a`: `decimal` (`int8_t*`)
  - `%A`: `decimal` (`int16_t*`)
  - `%p`: `hex` (`int64_t*`)
  - `%x`: `hex` (`int32_t*`)
  - `%c`: `char` (`char*`)
  - `%s`: `string` (`char[#max_next] / char *`)
 * 
 * ### Context Specifiers:
 * (non-ptr) specifiers don't return a result.
 * 
 - `%n`: `#max_next` (`size_t`)(non-ptr)
	Sets the buffer size of the next %s;
	Resets to `#max_next_all` after each string.
*
 - `%N`: `#max_next_all` (`size_t`)(non-ptr)
	Sets the buffer size of the next `%s`;
	Resets to `#max_next_all` after each string.
	default: `MAX_SIZE`
 - `%r`: current total read characters (`size_t*`)
 * ### Control Characters (without space):
 - `\ v` Skips at least one or more whitespace or stops on null-terminator;
	Fails if no whitespace or no null-terminator.
	Also used to specify 'no more expected values'.
 ### Returns
	The number of specifiers that were successfull completed.
 */
int	cl_sescanf(char *from, char *fmt, ...)
{
	va_list	list;
	int		found;

	va_start(list, fmt);
	found = cl_vsescanf(from, fmt, &list);
	va_end(list);
	return (found);
}

/*
#include <stdio.h>
int main(void)
{
	// size_t read = 12312;
	// int32_t num;
	// int fnd = cl_sescanf("  \t12345", "%r\v%d", &read, &num);
	// printf("read: %ld %d\n", read, fnd);

	char 	str[100];
	size_t read;
	size_t rest;
	int fnd = cl_sescanf("12345678", "%r%N%s.\v", &read, sizeof(str), str);
	printf("read: %d %ld %s\n", fnd, read, str);
}

typedef struct s_vec2i
{
	int x;
	int y;
} t_vec2i;

struct s_mod_door {
	char	name[50];
	char	tex[50];
	t_vec2i	pos;
	int		speed;
	char	locked;
	char	closed;
};

#include <stdio.h>
int main(void)
{
	struct s_mod_door door;
	int found = cl_sescanf("DR\vdooor01,./tex,10,25,6,C,L,", \
	"DR\v%s,%s,%u,%u,%u,%c,%c\v",
		&door.name,
		&door.tex,
		&door.pos.x,
		&door.pos.y,
		&door.speed,
		&door.locked,
		&door.closed
	);

	printf("%s\n%s\n%u\n%u\n%u\n%c\n%c",
		door.name,
		door.tex,
		door.pos.x,
		door.pos.y,
		door.speed,
		door.locked,
		door.closed);
	if (found != 7)
		printf("failed %d", found);
	return (0);
}
*/
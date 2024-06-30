/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_sescanf_handle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:56:00 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:53:43 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "sescanf_int.h"
#include "libcml.h"
#include "libcml_int.h"

static bool	sesc_should_stop(t_sescanf_ctx *ctx)
{
	const char	*data = ctx->current;

	if (data == NULL || *data == '\0')
		return (true);
	if (*ctx->fmt == '\0')
		return (false);
	if (sesc_nodelim_nextis(ctx, 'u'))
		return (isdigit(*data));
	else if (sesc_nodelim_nextis(ctx, 'd') || sesc_nodelim_nextis(ctx, 'i'))
		return (isdigit(*data) || \
		((*data == '-' || *data == '+') && isdigit(data[1])));
	else if (sesc_nodelim_nextis(ctx, 's') || sesc_nodelim_nextis(ctx, 'c') || \
		ctx->fmt[1] == '\v')
		return (isspace(*data));
	else
		return (*ctx->current == ctx->fmt[1]);
}

static int	sesc_handle_s(t_sescanf_ctx *ctx)
{
	char *const	dest = va_arg(*ctx->list, char *);
	size_t		i;

	i = 0;
	while (*ctx->current && i < (ctx->max_next - 1) && !sesc_should_stop(ctx))
	{
		dest[i] = *ctx->current;
		ctx->current++;
		i++;
	}
	ctx->max_next = ctx->max_next_all;
	dest[i] = '\0';
	return (0);
}

static int	sesc_handle_num(t_sescanf_ctx *ctx)
{
	void *const	dest = va_arg(*ctx->list, void*);
	char *const	start = ctx->current;
	char		*end;
	int			base;
	int64_t		value;

	base = 10;
	if (*ctx->type == 'x' || *ctx->type == 'p')
		base = 16;
	value = cl_strtol(ctx->current, &end, base);
	if (*ctx->type == 'p' || *ctx->type == 'D')
		*(int64_t *)dest = value;
	else if (*ctx->type == 'A')
		*(int16_t *)dest = value;
	else if (*ctx->type == 'a')
		*(int8_t *)dest = value;
	else
		*(int32_t *)dest = value;
	if ((*ctx->type == 'u' && value < 0) || \
		(*ctx->type == 'A' && value > INT16_MAX) || \
		(*ctx->type == 'a' && value > INT8_MAX)
	)
		return (-1);
	ctx->current = end;
	return (end - start);
}

static int	sesc_handle_c(t_sescanf_ctx *ctx)
{
	char *const	dest = va_arg(*ctx->list, char *);

	*dest = *ctx->current;
	if (*ctx->current)
		ctx->current++;
	return (1);
}

int64_t	sesc_handle_fmt_type(t_sescanf_ctx *ctx)
{
	if (*ctx->type == 'n')
		ctx->max_next = va_arg(*ctx->list, size_t);
	if (*ctx->type == 'N')
		ctx->max_next_all = va_arg(*ctx->list, size_t);
	if (*ctx->type == 'N')
		ctx->max_next = ctx->max_next_all;
	if (*ctx->type == 'r')
		ctx->read_ptr = va_arg(*ctx->list, int64_t *);
	if (*ctx->type == 'r')
		*ctx->read_ptr = 0;
	if (*ctx->type == 's')
		ctx->read = sesc_handle_s(ctx);
	else if (strchr("duixp", *ctx->type) != NULL)
		ctx->read = sesc_handle_num(ctx);
	else if (*ctx->type == 'c')
		ctx->read = sesc_handle_c(ctx);
	else if (strchr("rnN", *ctx->type) == NULL)
		return (-1);
	if (ctx->read_ptr && ctx->read > 0)
		*ctx->read_ptr += ctx->read;
	if (ctx->read >= 0)
		ctx->found++;
	return (ctx->read);
}

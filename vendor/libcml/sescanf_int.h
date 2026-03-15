/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sescanf_int.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:54:15 by clovell           #+#    #+#             */
/*   Updated: 2024/04/05 22:57:15 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdarg.h>
#include <inttypes.h>

typedef struct s_sescanf_ctx
{
	char	*fmt;
	char	*source;
	char	*current;
	char	*type;
	int		type_data;
	char	*next_type;
	va_list	*list;
	int64_t	read_total;
	int64_t	read;
	size_t	max_next;
	size_t	max_next_all;
	int		found;
	void	*context;
	int64_t	*read_ptr;
}	t_sescanf_ctx;

typedef int	(*t_fn_sscanf_handle)(char *data, char *fmt_next, void *dest);

char	sesc_fmt_gettype(char *fmt, int step, char **type_start);
bool	sesc_nodelim_nextis(t_sescanf_ctx *ctx, char type);

int64_t	sesc_handle_fmt_type(t_sescanf_ctx *ctx);
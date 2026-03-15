/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcml.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 12:41:29 by clovell           #+#    #+#             */
/*   Updated: 2024/05/06 23:06:38 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCML_H
# define LIBCML_H
# include <stddef.h>
# include <inttypes.h>
# include <stdarg.h>

typedef unsigned char	t_uchar;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

void	*cl_memsetf32(void *b, float c, size_t len);
void	*cl_memsetf64(void *b, double c, size_t len);
void	*cl_memset(void *b, int c, size_t n);

size_t	cl_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	cl_strlcat(char *dst, const char *src, size_t dstsize);

char	*cl_strdup(const char *s1);
char	*cl_strjoin(char const *s1, char const *s2);
char	**cl_split(char const *s, char c);
char	*cl_itoa(int n);
char	*cl_ltoa(long long int n);
char	*cl_ftoa(double n, int precision);
size_t	cl_intlen(int64_t	nb);
size_t	cl_ltostr(int64_t num, char *str);

char	*cl_v3toa(double v[3]);
char	*cl_v2toa(double v[2]);
char	*cl_v3itoa(int v[3]);
char	*cl_v2itoa(int v[2]);

char	*cl_strfmt(char *fmt, ...);
int64_t	cl_strtol(char *start, char **end, uint32_t base);
int		cl_vsescanf(char *src, char *fmt, va_list *list);
int		cl_sescanf(char *from, char *fmt, ...);

#endif
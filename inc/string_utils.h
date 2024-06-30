/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:24:49 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:17:33 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRING_UTILS_H
# define STRING_UTILS_H

int		strlen_nl(const char *str);
int		cl_strcmp(char *s1, char *s2);
int		cl_isspace(int c);
void	remove_spaces(char *str);
int		is_line(const char *line);
#endif
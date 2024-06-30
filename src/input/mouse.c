/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 16:27:16 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:28:05 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "app.h"

#ifdef __linux__

void	mouse_get_pos(t_app *app, int *x, int *y)
{
	int	x1 = 0;
	int	y1 = 0;
	(void)app;

	//unknown_get_mouse(app->win, &x1, &y1);
	if (x)
		*x = x1;
	if (y)
		*y = y1;
}
#else

void	mouse_get_pos(t_app *app, int *x, int *y)
{
	int	x1 = 0;
	int	y1 = 0;
	(void)app;

	//unknown_mouse_get_pos(app->win, &x1, &y1);
	if (x)
		*x = x1;
	if (y)
		*y = y1;
}
#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 19:43:57 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:11:27 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H
# include "input.h"
# include "texture.h"
# include "app.h"
# include "random.h"

// FORWARD DECLARE
typedef struct s_task			t_task;
typedef struct s_game			t_game;

struct s_game
{
	t_app			app;
	t_texture		rt0;
};


/* util/time.c */
int64_t	time_get_ms(void);
#endif

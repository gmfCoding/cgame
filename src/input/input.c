/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 21:36:29 by clovell           #+#    #+#             */
/*   Updated: 2024/04/08 16:20:07 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include <stddef.h>
#include "state.h"
#include "vector2i.h"
#include "input.h"

const int	g_size[] = {
	sizeof((t_inputctx){0}.mouse_state),
	sizeof((t_inputctx){0}.key_state),
	sizeof((t_inputctx){0}.special_state),
	0,
};

bool	input_keydown(t_inputctx *input, int key)
{
	uint8_t	*state;

	state = input_get_state(input, &key);
	return ((state[key] & KEY_MASK_PRESS) == KEY_MASK_PRESS);
}

bool	input_keyup(t_inputctx *input, int key)
{
	uint8_t	*state;

	state = input_get_state(input, &key);
	return ((state[key] & KEY_MASK_RELEASE) == KEY_MASK_RELEASE);
}

bool	input_keyheld(t_inputctx *input, int key)
{
	uint8_t	*state;

	state = input_get_state(input, &key);
	return ((state[key] & KEY_MASK_PRESS) == KEY_MASK_PRESS || \
	((state[key] & KEY_MASK_RELEASE) != KEY_MASK_RELEASE && \
	(state[key] & KEY_MASK_PREV) == KEY_MASK_PREV));
}

void	input_process(t_inputctx *input)
{
	uint8_t *const	states[] = {
		input->mouse_state, input->key_state,
		input->special_state, NULL,
	};
	int				i;
	int				j;

	j = -1;
	while (states[++j])
	{
		i = -1;
		while (++i < g_size[j])
		{
			if ((states[j][i] & KEY_MASK_PRESS) == KEY_MASK_PRESS)
			{
				states[j][i] &= ~KEY_MASK_PRESS;
				states[j][i] |= KEY_MASK_PREV;
			}
			if ((states[j][i] & KEY_MASK_RELEASE) == KEY_MASK_RELEASE)
				states[j][i] = 0;
		}
	}
	input->mouse_prev = input->mouse;
}

uint8_t	*input_get_state(t_inputctx *input, int *key)
{
	uint8_t	*state;

	state = input->key_state;
	if ((*key & KEY_CMD_MASK) == KEY_CMD_MASK)
	{
		*key &= ~KEY_CMD_MASK;
		state = input->special_state;
	}
	else if ((*key & KEY_MB_MASK) == KEY_MB_MASK)
	{
		*key &= ~KEY_MB_MASK;
		state = input->mouse_state;
	}
	return (state);
}

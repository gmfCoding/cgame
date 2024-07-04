/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 21:40:25 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:14:02 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "state.h"
#include "input.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

t_inputctx *glfw_input_context;

void input_cb_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	printf("%d %d %d %d\n", key, scancode, action, mods);

	(void)window;
	(void)scancode;
	(void)mods;
	if (key > KEY_REG_MAX)
		return;
	uint8_t	*state;
	// uint8_t	*state_mod;

	state = glfw_input_context->key_state;
	// state_mod = glfw_input_context->special_state;
	if (action == GLFW_RELEASE)
		state[key] |= KEY_MASK_RELEASE;
	else if (action == GLFW_PRESS)
		state[key] |= KEY_MASK_PRESS;
	else
		return;

	// if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
	// 	state_mod[GLFW_MOD_SHIFT] = action;
	// if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
	// 	state_mod[GLFW_MOD_CONTROL] = action;
	// if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
	// 	state_mod[GLFW_MOD_ALT] = action;
	// if ((mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER)
	// 	state_mod[GLFW_MOD_SUPER] = action;
	// if ((mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK)
	// 	state_mod[GLFW_MOD_CAPS_LOCK] = action;
	// if ((mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK)
	// 	state_mod[GLFW_MOD_NUM_LOCK] = action;
}


void input_cb_mousekey(GLFWwindow* window, int button, int action, int mods)
{	
	(void)window;
	(void)mods;
	if (button > 5)
		return;
	uint8_t	*state;

	state = glfw_input_context->mouse_state;
	if (action == GLFW_RELEASE)
		state[button] |= KEY_MASK_RELEASE;
	else if (action == GLFW_PRESS)
		state[button] |= KEY_MASK_PRESS;
}

void	input_cb_mouse_move(GLFWwindow* window, double x, double y)
{
	(void)window;
	glfw_input_context->mouse = v2inew(x, y);
	glfw_input_context->mousef = v2new(x, y);
}

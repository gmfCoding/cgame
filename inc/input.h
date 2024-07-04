/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:18:00 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 04:26:59 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H
# include <stdbool.h>
# include <inttypes.h>
# include "keys.h"
# include "structs/s_inputcontext.h"

typedef struct s_inputstate	t_inputstate;
typedef struct s_inputctx	t_inputctx;
typedef struct s_app		t_app;

extern t_inputctx *glfw_input_context;

bool	input_keydown(t_inputctx *input, int key);
bool	input_keyup(t_inputctx *input, int key);
bool	input_keyheld(t_inputctx *input, int key);
void	input_process(t_inputctx *input);

void	mouse_get_pos(t_app *app, int *x, int *y);
uint8_t	*input_get_state(t_inputctx *input, int *key);

void	input_setup(void *win, t_inputctx *input);

typedef struct GLFWwindow GLFWwindow;

void    input_cb_mousekey(GLFWwindow* window, int button, int action, int mods);
void    input_cb_key(GLFWwindow* window, int key, int scancode, int action, int mods);
void	input_cb_mouse_move(GLFWwindow* window, double x, double y);

#endif

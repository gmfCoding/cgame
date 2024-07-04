/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 21:10:21 by clovell           #+#    #+#             */
/*   Updated: 2024/07/01 03:13:13 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <GLFW/glfw3.h>
# include <stdbool.h>

# define KEY_MB_MASK	0x00FF0000
# define KEY_CMD_MASK	0x0000FF00

# define KEY_MASK_PRESS     0b1 // Has it been pressed?
# define KEY_MASK_PREV      0b10 // Was it held on the previous frame?
# define KEY_MASK_RELEASE   0b100 // Has it been released?

# define MB_LEFT		0x00FF0001
# define MB_RIGHT		0x00FF0002
# define MB_MIDDLE		0x00FF0003
# define MB_SCRLUP		0x00FF0004
# define MB_SCRLDW		0x00FF0005

#  define KEY_REG_MAX	350
#  define KEY_SPACE		GLFW_KEY_SPACE
#  define KEY_APOSTROPH	GLFW_KEY_APOSTROPHE
#  define KEY_COMMA		GLFW_KEY_COMMA
#  define KEY_PERIOD	GLFW_KEY_PERIOD
#  define KEY_MINUS		GLFW_KEY_MINUS
#  define KEY_EQUALS	GLFW_KEY_EQUAL
#  define KEY_FSLASH	GLFW_KEY_SLASH
#  define KEY_BSLASH	GLFW_KEY_BACKSLASH
#  define KEY_0			GLFW_KEY_0
#  define KEY_1			GLFW_KEY_1
#  define KEY_2			GLFW_KEY_2
#  define KEY_3			GLFW_KEY_3
#  define KEY_4			GLFW_KEY_4
#  define KEY_5			GLFW_KEY_5
#  define KEY_6			GLFW_KEY_6
#  define KEY_7			GLFW_KEY_7
#  define KEY_8			GLFW_KEY_8
#  define KEY_9			GLFW_KEY_9
#  define KEY_SEMICOLON	GLFW_KEY_SEMICOLON
#  define KEY_TILDA		GLFW_KEY_GRAVE_ACCENT
#  define KEY_SQBLEFT	GLFW_KEY_LEFT_BRACKET
#  define KEY_SQBRIGHT	GLFW_KEY_RIGHT_BRACKET
#  define KEY_TAB		GLFW_KEY_TAB
#  define KEY_A			GLFW_KEY_A
#  define KEY_B			GLFW_KEY_B
#  define KEY_C			GLFW_KEY_C
#  define KEY_D			GLFW_KEY_D
#  define KEY_E			GLFW_KEY_E
#  define KEY_F			GLFW_KEY_F
#  define KEY_G         GLFW_KEY_G
#  define KEY_H         GLFW_KEY_H
#  define KEY_I         GLFW_KEY_I
#  define KEY_J         GLFW_KEY_J
#  define KEY_K         GLFW_KEY_K
#  define KEY_L         GLFW_KEY_L
#  define KEY_M         GLFW_KEY_M
#  define KEY_N         GLFW_KEY_N
#  define KEY_O         GLFW_KEY_O
#  define KEY_P         GLFW_KEY_P
#  define KEY_Q         GLFW_KEY_Q
#  define KEY_R         GLFW_KEY_R
#  define KEY_S         GLFW_KEY_S
#  define KEY_T         GLFW_KEY_T
#  define KEY_U         GLFW_KEY_U
#  define KEY_V         GLFW_KEY_V
#  define KEY_W         GLFW_KEY_W
#  define KEY_X         GLFW_KEY_X
#  define KEY_Y         GLFW_KEY_Y
#  define KEY_Z         GLFW_KEY_Z

#  define KEY_BACKSPACE	GLFW_KEY_BACKSPACE
#  define KEY_ENTER		GLFW_KEY_ENTER

#  define KEY_LCTRL		GLFW_KEY_LEFT_CONTROL
#  define KEY_RCTRL		GLFW_KEY_RIGHT_CONTROL

#  define KEY_CAPS		GLFW_KEY_CAPS_LOCK

#  define KEY_LSHIFT	GLFW_KEY_LEFT_SHIFT
#  define KEY_RSHIFT	GLFW_KEY_RIGHT_SHIFT

#  define KEY_LALT		GLFW_KEY_LEFT_ALT
#  define KEY_RALT		GLFW_KEY_RIGHT_ALT

#  define KEY_DEL		GLFW_KEY_DELETE

#  define KEY_ESC		GLFW_KEY_ESCAPE

#  define KEY_LARROW	GLFW_KEY_LEFT
#  define KEY_UARROW	GLFW_KEY_UP
#  define KEY_RARROW	GLFW_KEY_RIGHT
#  define KEY_DARROW	GLFW_KEY_DOWN

#  define KEY_F1		GLFW_KEY_F1
#  define KEY_F2		GLFW_KEY_F2
#  define KEY_F3		GLFW_KEY_F3
#  define KEY_F4		GLFW_KEY_F4
#  define KEY_F5		GLFW_KEY_F5
#  define KEY_F6		GLFW_KEY_F6
#  define KEY_F7		GLFW_KEY_F7
#  define KEY_F8		GLFW_KEY_F8
#  define KEY_F9		GLFW_KEY_F9
#  define KEY_F10		GLFW_KEY_F10
#  define KEY_F11		GLFW_KEY_F11
#  define KEY_F12		GLFW_KEY_F12
#  define KEY_F14       GLFW_KEY_F14

/* UNVERIFIED */
#  define KEY_MENU		GLFW_KEY_MENU
#  define KEY_HOME		GLFW_KEY_HOME

#  define KEY_PGUP	    GLFW_KEY_PAGE_UP
#  define KEY_PGDW		GLFW_KEY_PAGE_DOWN
#  define KEY_END		GLFW_KEY_END
#  define KEY_INS		GLFW_KEY_INSERT

#  define KEY_SCRLCK	GLFW_KEY_SCROLL_LOCK

#  define KEY_NP_LOCK   GLFW_KEY_NUM_LOCK /* TODO: is it the same as scroll? */
#  define KEY_NP_1		GLFW_KEY_KP_1
#  define KEY_NP_2		GLFW_KEY_KP_2
#  define KEY_NP_3		GLFW_KEY_KP_3
#  define KEY_NP_4		GLFW_KEY_KP_4
#  define KEY_NP_5		GLFW_KEY_KP_5
#  define KEY_NP_6		GLFW_KEY_KP_6
#  define KEY_NP_7		GLFW_KEY_KP_7
#  define KEY_NP_8		GLFW_KEY_KP_8
#  define KEY_NP_9		GLFW_KEY_KP_9
#  define KEY_NP_0		GLFW_KEY_KP_0
#  define KEY_NP_DIV	GLFW_KEY_KP_DIVIDE
#  define KEY_NP_ADD	GLFW_KEY_KP_ADD
#  define KEY_NP_MULT	GLFW_KEY_KP_MULTIPLY
#  define KEY_NP_SUB	GLFW_KEY_KP_SUBTRACT
#  define KEY_NP_ENTER	GLFW_KEY_KP_ENTER
#  define KEY_NP_PERIOD	0
#  define KEY_NP_EQUAL	0

void		key_get_mapkey(const int **array, int *size);
void		key_get_mapstr(const char *const **array, int *size, bool name);
const char	*key_get_keystr(int key, bool name);
#endif

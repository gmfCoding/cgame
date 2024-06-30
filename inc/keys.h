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

# ifdef __linux__
#  define KEY_REG_MAX	127
#  define KEY_SPACE		32
#  define KEY_APOSTROPH	39
#  define KEY_COMMA		44
#  define KEY_PERIOD	46
#  define KEY_MINUS		45
#  define KEY_EQUALS	61
#  define KEY_FSLASH	47
#  define KEY_BSLASH	92
#  define KEY_0			48
#  define KEY_1			49
#  define KEY_2			50
#  define KEY_3			51
#  define KEY_4			52
#  define KEY_5			53
#  define KEY_6			54
#  define KEY_7			55
#  define KEY_8			56
#  define KEY_9			57
#  define KEY_SEMICOLON	59
#  define KEY_TILDA		96
#  define KEY_SQBLEFT	91
#  define KEY_SQBRIGHT	93
#  define KEY_TAB		65289
#  define KEY_A			97
#  define KEY_B			98
#  define KEY_C			99
#  define KEY_D			100
#  define KEY_E			101
#  define KEY_F			102
#  define KEY_G			103
#  define KEY_H			104
#  define KEY_I			105
#  define KEY_J			106
#  define KEY_K			107
#  define KEY_L			108
#  define KEY_M			109
#  define KEY_N			110
#  define KEY_O			111
#  define KEY_P			112
#  define KEY_Q			113
#  define KEY_R			114
#  define KEY_S			115
#  define KEY_T			116
#  define KEY_U			117
#  define KEY_V			118
#  define KEY_W			119
#  define KEY_X			120
#  define KEY_Y			121
#  define KEY_Z			122

#  define KEY_BACKSPACE	65288
#  define KEY_ENTER		65293

#  define KEY_LCTRL		65507
#  define KEY_RCTRL		65508

#  define KEY_CAPS		65509

#  define KEY_LSHIFT	65505
#  define KEY_RSHIFT	65506

#  define KEY_LALT		65513
#  define KEY_RALT		65514

#  define KEY_DEL		65535

#  define KEY_ESC		65307

#  define KEY_LARROW	65361
#  define KEY_UARROW	65362
#  define KEY_RARROW	65363
#  define KEY_DARROW	65364

#  define KEY_F1		65470
#  define KEY_F2		65471
#  define KEY_F3		65472
#  define KEY_F4		65473
#  define KEY_F5		65474
#  define KEY_F6		65475
#  define KEY_F7		65476
#  define KEY_F8		65477
#  define KEY_F9		65478
#  define KEY_F10		65479
#  define KEY_F11		65480
#  define KEY_F12		65481
#  define KEY_F14       65483

/* UNVERIFIED */
#  define KEY_MENU		65383
#  define KEY_HOME		65360

#  define KEY_PGUP		65365
#  define KEY_PGDW		65366
#  define KEY_END		65367
#  define KEY_INS		65379

#  define KEY_SCRLCK	65407

#  define KEY_NP_LOCK   65407 /* TODO: is it the same as scroll? */
#  define KEY_NP_1		65436
#  define KEY_NP_2		65433
#  define KEY_NP_3		65435
#  define KEY_NP_4		65430
#  define KEY_NP_5		65437
#  define KEY_NP_6		65432
#  define KEY_NP_7		65429
#  define KEY_NP_8		65431
#  define KEY_NP_9		65434
#  define KEY_NP_0		65438
#  define KEY_NP_DIV	65455
#  define KEY_NP_ADD	65451
#  define KEY_NP_MULT	65450
#  define KEY_NP_SUB	65453
#  define KEY_NP_ENTER	65421
#  define KEY_NP_PERIOD	65439
#  define KEY_NP_EQUAL	0
# else
#  define KEY_REG_MAX	272
#  define KEY_SPACE		49
#  define KEY_APOSTROPH	39
#  define KEY_COMMA		43
#  define KEY_PERIOD	47
#  define KEY_MINUS		27
#  define KEY_EQUALS	24
#  define KEY_FSLASH	44
#  define KEY_BSLASH	42
#  define KEY_0			29
#  define KEY_1			18
#  define KEY_2			19
#  define KEY_3			20
#  define KEY_4			21
#  define KEY_5			23
#  define KEY_6			22
#  define KEY_7			26
#  define KEY_8			28
#  define KEY_9			25
#  define KEY_SEMICOLON	41
#  define KEY_TILDA		50
#  define KEY_SQBLEFT	33
#  define KEY_SQBRIGHT	30
#  define KEY_TAB		48
#  define KEY_A			0
#  define KEY_B			11
#  define KEY_C			8
#  define KEY_D			2
#  define KEY_E			14
#  define KEY_F			3
#  define KEY_G			5
#  define KEY_H			4
#  define KEY_I			34
#  define KEY_J			38
#  define KEY_K			40
#  define KEY_L			37
#  define KEY_M			46
#  define KEY_N			45
#  define KEY_O			31
#  define KEY_P			35
#  define KEY_Q			12
#  define KEY_R			15
#  define KEY_S			1
#  define KEY_T			17
#  define KEY_U			32
#  define KEY_V			9
#  define KEY_W			13
#  define KEY_X			7
#  define KEY_Y			16
#  define KEY_Z			6

#  define KEY_BACKSPACE	51
#  define KEY_ENTER		36

#  define KEY_LCTRL		256
#  define KEY_RCTRL		269

#  define KEY_CAPS		272

#  define KEY_LSHIFT	257
#  define KEY_RSHIFT	258

#  define KEY_LALT		261
#  define KEY_RALT		262

#  define KEY_DEL		117

#  define KEY_ESC		53

#  define KEY_LARROW	123
#  define KEY_UARROW	126
#  define KEY_RARROW	124
#  define KEY_DARROW	125

#  define KEY_F1		122
#  define KEY_F2		120
#  define KEY_F3		99
#  define KEY_F4		118
#  define KEY_F5		96
#  define KEY_F6		97
#  define KEY_F7		98
#  define KEY_F8		100
#  define KEY_F9		101
#  define KEY_F10		109
#  define KEY_F11		103
#  define KEY_F12		111
#  define KEY_F14       107

#  define KEY_MENU		KEY_F9
#  define KEY_HOME		115
#  define KEY_PGUP		116
#  define KEY_PGDW		121
#  define KEY_END		119
/* HELP KEY REMAP (N/A)*/
#  define KEY_INS		114 

/* F14 REMAP (N/A)*/
#  define KEY_SCRLCK	KEY_F14

#  define KEY_NP_LOCK   71 // CLEAR
#  define KEY_NP_1		83
#  define KEY_NP_2		84
#  define KEY_NP_3		85
#  define KEY_NP_4		86
#  define KEY_NP_5		87
#  define KEY_NP_6		88
#  define KEY_NP_7		89
#  define KEY_NP_8		91
#  define KEY_NP_9		92
#  define KEY_NP_0		82
#  define KEY_NP_DIV	75
#  define KEY_NP_ADD	69
#  define KEY_NP_MULT	67
#  define KEY_NP_SUB	78
#  define KEY_NP_ENTER	76
#  define KEY_NP_PERIOD	65
#  define KEY_NP_EQUAL	81
# endif

void		key_get_mapkey(const int **array, int *size);
void		key_get_mapstr(const char *const **array, int *size, bool name);
const char	*key_get_keystr(int key, bool name);
#endif

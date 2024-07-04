#ifndef S_INPUTCONTEXT_H
# define S_INPUTCONTEXT_H

# include <inttypes.h>
# include "keys.h"
# include "vector2.h"
# include "vector2i.h"

#ifndef KEY_REG_MAX
#define KEY_REG_MAX 350
#endif

struct s_inputctx
{
	uint8_t	key_state[KEY_REG_MAX];
	uint8_t	special_state[256];
	uint8_t	mouse_state[5];

	t_vec2i	mouse;
	t_vec2i	mouse_prev;
	t_vec2	mousef;

	double prev_mouse_x;
	double prev_mouse_y;

	double curr_mouse_x;
	double curr_mouse_y;

	double mouse_x_delta;
	double mouse_y_delta;

	bool first_time;
	bool ignore_next_delta;

	int		frame;
};
#endif
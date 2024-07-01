/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 08:27:18 by clovell           #+#    #+#             */
/*   Updated: 2024/05/08 14:31:05 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CONFIG_H
# define CONFIG_H

// How many possible depths can a raycast contain?
# define MAX_DEPTHS 5

// Screen resolution X
# define SCR_WIDTH 960
// Screen resolution Y
# define SCR_HEIGHT 960

# define R_SCALE 1

// Render resolution X
// # define R_WIDTH (SCR_WIDTH/R_SCALE)
# define R_WIDTH 960
// Render resolution Y
// # define R_HEIGHT (SCR_HEIGHT/R_SCALE)
# define R_HEIGHT 960

# define MOUSE_SENSITIVITY 2 

# define RT0_WIDTH SCR_WIDTH
# define RT0_HEIGHT SCR_HEIGHT

# define TARGET_FRAME_RATE 60.0
# define R_TFR TARGET_FRAME_RATE

#endif

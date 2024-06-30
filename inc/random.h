/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:17:37 by clovell           #+#    #+#             */
/*   Updated: 2024/02/07 16:42:04 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef RANDOM_H
# define RANDOM_H
# include <inttypes.h>
# define MRAND_MAX 32767
# define MRAND_MAXF 32767.0

typedef struct s_rand
{
	uint64_t	next;
}				t_rand;

void	msrand(t_rand *rand, uint64_t seed);
int		mrand(t_rand *rand);
#endif
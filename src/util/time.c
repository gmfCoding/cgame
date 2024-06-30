/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmordaun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:14:19 by kmordaun          #+#    #+#             */
/*   Updated: 2024/05/03 17:48:02 by kmordaun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <inttypes.h>
#include <stddef.h>
#include <unistd.h>

// TODO: Remove for mandatory build
int64_t	time_get_ms(void)
{
	int64_t			ms;
	struct timeval	time;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000.0;
	ms += time.tv_usec / 1000.0;
	return (ms);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 00:42:17 by clovell           #+#    #+#             */
/*   Updated: 2024/05/02 00:43:52 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ITER_H
# define ITER_H

# include <limits.h>
# include <stdint.h>
# include <inttypes.h>

# define ITER_UCHAR_START UCHAR_MAX
# define ITER_USHORT_START USHRT_MAX
# define ITER_UINT_START UINT_MAX
# define ITER_SIZET_START SIZE_MAX

# define ITER_UINT8_START UINT8_MAX
# define ITER_UINT16_START UINT16_MAX
# define ITER_UINT32_START UINT32_MAX
# define ITER_UINT64_START UINT64_MAX

# define ITER_INT8_START -1
# define ITER_INT16_START -1
# define ITER_INT32_START -1
# define ITER_INT64_START -1

# define ITER_INT_START -1
# define ITER_SSIZET_START -1

#endif
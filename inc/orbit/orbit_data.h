/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orbit_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clovell <clovell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:57:06 by clovell           #+#    #+#             */
/*   Updated: 2024/02/13 00:24:10 by clovell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ORBIT_DATA_H
# define ORBIT_DATA_H
# include "vector3.h"
# include "vector2.h"

typedef struct s_kep_path			t_kep_path;
typedef struct s_kep_ang			t_kep_ang;
typedef struct s_orb_body_def		t_orb_body;
typedef struct s_orbital_object		t_orb_obj;
typedef enum e_orb_cart_ref_frame	t_ocrf;
typedef struct s_orb_cart			t_orb_cart;

// typedef enum e_kep_ang_value t_kep_ang_value;
// enum e_kep_ang_value
// {
// 	KEP_EA,
// 	KEP_M,
// 	KEP_TA
// };

/*
 OCRF_ORF: Orbital Reference Frame
	The reference frame where the periapsis lines on the -x axis extent
	Orbital elements do not effect the shape of this reference frame (exc ecc).

 OCRF_IRF: Inertial Reference Frame
	The reference frame where the orbit is transformed by orbital elements.
	All orbital elements effect this reference frame. except on gimbal lock.
*/
enum e_orb_cart_ref_frame
{
	OCRF_ORF,
	OCRF_IRF
};

struct s_orb_cart
{
	t_ocrf	ref;
	t_vec3	pos;
	t_vec3	vel;
};

struct s_orb_body_def
{
	double	mass;
	double	radius;
	double	density;
	double	u;
};

struct s_kep_path
{
	t_orb_body	*parent;
	t_orb_body	*self;
	double		sma;
	double		inc;
	double		lan;
	double		aop;
	double		ecc;
	double		sgp_u;
};

struct s_kep_ang
{
	struct
	{
		double	time0;
		double	mna0;
	} s_0;
	double	time;
	double	mna;
	double	ea;
	double	ta;
};

struct	s_orbital_object
{
	t_kep_path	path;
	t_kep_ang	ang;
	t_orb_cart	cart;

	t_orb_body	parent;
	t_orb_body	self;
};

typedef struct s_orb_gen
{
	t_vec2	sma;
	t_vec2	ecc;
	t_vec2	inc;
	t_vec2	aop;
	t_vec2	lan;
}				t_orb_gen;
#endif
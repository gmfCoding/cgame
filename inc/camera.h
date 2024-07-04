#ifndef CAMERA_H
#define CAMERA_H
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include "transform.h"

typedef struct s_move
{
    bool Left;
    bool Right;
    bool Forward;
    bool Backwards;
    bool Down;
    bool Up;
} t_move;

typedef struct s_camera
{
	t_transform transform;

    mat4 view;
    mat4 projection;
    vec3 direction;

	mat4 premultPV;

    float yaw;
    float pitch;

} t_camera;
typedef struct s_inputctx t_inputctx;
#define CAMERA_DEFAULT (t_camera){.yaw = 225.0, .view = GLM_MAT4_IDENTITY_INIT, .projection = GLM_MAT4_IDENTITY_INIT, .direction = {0,0,1}}

void camera_view_update(t_camera *camera);
void camera_control_look(t_camera *camera, t_inputctx *input);
void camera_control(t_camera *camera, t_move *move, float deltaTime);
void camera_init(t_camera *camera, float fov, float aspect, float near, float far);

#endif
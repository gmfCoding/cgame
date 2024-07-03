#include "camera.h"

void camera_init(t_camera *camera, float fov, float aspect, float near, float far)
{
	glm_perspective(
        glm_rad(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        aspect,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        near,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        far,             // Far clipping plane. Keep as little as possible.
		camera->projection 
    );

    camera->direction[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->direction[1] = sin(glm_rad(camera->pitch));
    camera->direction[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(camera->direction);
}


void camera_view_update(t_camera *camera)
{
    vec3 up = {0.0f, 1.0f, 0.0f};

    camera->direction[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->direction[1] = sin(glm_rad(camera->pitch));
    camera->direction[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(camera->direction);

	vec3 centre;
	glm_vec3_add(camera->transform.position, camera->direction, centre);
    glm_lookat(
        camera->transform.position,
		centre,
        up,
		camera->view
    );

	glm_mat4_mul(camera->projection, camera->view, camera->premultPV);
}


void camera_control(t_camera *camera, t_move *move, float deltaTime)
{
	float moveSpeed = 6.6f * deltaTime;
	float *pos = &camera->transform.position[0];

	vec3 cameraLeft = {0};
	glm_vec3_cross((vec3){0,1,0}, camera->direction, cameraLeft);
	glm_normalize(cameraLeft);

	vec3 cameraLeftN;
	glm_vec3_copy(cameraLeft, cameraLeftN);
	glm_vec3_inv(cameraLeftN);
	
	vec3 cameraUp = {0};
	glm_vec3_cross(camera->direction, cameraLeftN, cameraUp);
	glm_normalize(cameraUp);
	glm_vec3_inv(cameraUp);

	glm_vec3_scale(cameraLeft, moveSpeed, cameraLeft);
	glm_vec3_scale(cameraUp, moveSpeed, cameraUp);
	vec3 forward;
	glm_vec3_copy(camera->direction, forward);
	glm_vec3_scale(forward, moveSpeed, forward);

	if(move->Left) 
		glm_vec3_add(pos, cameraLeft, pos);
	if(move->Right)
		glm_vec3_sub(pos, cameraLeft, pos);
	if(move->Forward) 
		glm_vec3_add(pos, forward, pos);
	if(move->Backwards) 
		glm_vec3_sub(pos, forward, pos);
	if(move->Up) 
		glm_vec3_add(pos, cameraUp, pos);
	if(move->Down) 
		glm_vec3_sub(pos, cameraUp, pos);
}
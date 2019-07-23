#pragma once
#include "Core.h"
#include "vec3.h"
#include "mat4.h"


MARS_API struct
{
	Mars::vec3 camera_position;
	Mars::vec3 camera_front;
	Mars::vec3 camera_up;
	Mars::vec3 camera_right;
	Mars::vec3 camera_target;
	Mars::vec3 camera_direction;
	Mars::mat4 view;
	f32 yaw;
	f32 pitch;
} camera_data;

namespace Mars
{
	void CreateCamera()
	{
		camera_data.camera_position = vec3(0.f, 0.f, 3.f);
		camera_data.camera_front = vec3(0.f, 0.f, -1.f);
		camera_data.camera_target = vec3(0.f);
		camera_data.camera_direction = vec3::Normalize(camera_data.camera_position - camera_data.camera_target);

		vec3 up = vec3(0.f, 1.f, 0.f);
		camera_data.camera_right = vec3::Normalize(vec3::Cross(up, camera_data.camera_direction));
		camera_data.camera_up = vec3::Cross(camera_data.camera_direction, camera_data.camera_right);

		camera_data.yaw = -90.f;
		camera_data.pitch = 0.f;
	}
}

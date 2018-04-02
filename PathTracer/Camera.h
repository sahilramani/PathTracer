#pragma once

#include "ray.h"

class Camera
{
public:
	Camera(Vec3 position, Vec3 lookat, Vec3 up, float fov, float aspect)
		: m_origin(position)
	{
		Vec3 u, v, w;

		w = unit_vector(position - lookat);
		u = unit_vector(cross(up, w));
		v = unit_vector(cross(w, u));

		float theta = fov * float(M_PI) / 180.f;
		float half_height = tanf(theta / 2.f);
		float half_width = aspect * half_height;

		m_lower_left_corner = position - half_width * u - half_height * v - w;

		m_horizontal = 2.f * half_width * u;
		m_vertical = 2.f * half_height * v;
	}

	Ray get_ray(float u, float v) { return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin); }

private:
	Vec3 m_lower_left_corner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
	Vec3 m_origin;
};
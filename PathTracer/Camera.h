#pragma once

#include "ray.h"

class Camera
{
public:
	Camera()
	{
		m_lower_left_corner = Vec3(-2.f, -1.f, -1.f);
		m_horizontal = Vec3(4.f, 0.f, 0.f);
		m_vertical = Vec3(0.f, 2.f, 0.f);
		m_origin = Vec3(0.f, 0.f, 0.f);
	}

	Ray get_ray(float u, float v) { return Ray(m_origin, m_lower_left_corner + u*m_horizontal + v*m_vertical - m_origin); }

private:
	Vec3 m_lower_left_corner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
	Vec3 m_origin;
};
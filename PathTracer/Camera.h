#pragma once

#include "ray.h"

Vec3 random_in_unit_disk()
{
	Vec3 p;
	do
	{
		p = 2.f * Vec3(RAND_FLOAT, RAND_FLOAT, 0.f) - Vec3(1.f, 1.f, 0.f);
	} while (dot(p, p) >= 1.f);

	return p;
}

class Camera
{
public:
	Camera(Vec3 position, Vec3 lookat, Vec3 up, float fov, float aspect, float aperture, float focus_distance)
		: m_origin(position)
		, m_lens_radius(aperture/2.f)
	{
		Vec3 u, v, w;

		w = unit_vector(position - lookat);
		u = unit_vector(cross(up, w));
		v = unit_vector(cross(w, u));

		float theta = fov * float(M_PI) / 180.f;
		float half_height = tanf(theta / 2.f);
		float half_width = aspect * half_height;

		m_lower_left_corner = position - half_width * focus_distance* u - half_height * focus_distance * v - focus_distance * w;

		m_horizontal = 2.f * focus_distance * half_width * u;
		m_vertical = 2.f * focus_distance * half_height * v;
	}

	Ray get_ray(float u, float v) 
	{ 
		Vec3 rand_point = m_lens_radius * random_in_unit_disk();
		Vec3 offset(u * rand_point.x(), v * rand_point.y(), 0.f);
		return Ray(m_origin + offset, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin - offset); 
	}

private:
	Vec3 m_lower_left_corner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
	Vec3 m_origin;

	Vec3 m_u, m_v, m_w;
	float m_lens_radius;
};
#pragma once

#include "RayTarget.h"
#include "vec3.h"

class Material;

class Sphere : public RayTarget
{
public:
	Sphere() {}
	Sphere(Vec3 center, float radius, Material* mat)
		: m_center(center)
		, m_radius(radius)
		, m_material(mat)
	{}

	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& hit) const;

private:
	Vec3 m_center;
	float m_radius;
	Material* m_material;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& hit) const
{
	Vec3 oc = r.origin() - m_center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - m_radius * m_radius;
	float discriminant = b * b - a * c;
	if (discriminant >= 0.0f)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			hit.t = temp;
			hit.p = r.point_at_param(temp);
			hit.normal = (hit.p - m_center) / m_radius;
			hit.material = m_material;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			hit.t = temp;
			hit.p = r.point_at_param(temp);
			hit.normal = (hit.p - m_center) / m_radius;
			hit.material = m_material;
			return true;
		}
	}

	return false;
}
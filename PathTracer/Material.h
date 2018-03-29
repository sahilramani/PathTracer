#pragma once

#include "ray.h"
#include "RayTarget.h"

#define RAND_FLOAT ((float)rand()/(float)((unsigned int)RAND_MAX+1))

class Material
{
public:
	virtual bool scatter(const Ray& ray, const hit_record& hit, Vec3& attenuation, Ray& scattered) const = 0;

protected:
	static Vec3 random_in_unit_sphere()
	{
		Vec3 p;
		do
		{
			p = 2.0f * Vec3(RAND_FLOAT, RAND_FLOAT, RAND_FLOAT) - Vec3(1.0f, 1.0f, 1.0f);
		} while (p.squared_length() >= 1.0f);
		return p;
	}

	static Vec3 reflect(const Vec3& v, const Vec3& n) { return v - 2.f * dot(v, n) * n; }
};

class Lambertian : public Material
{
public:
	Lambertian(Vec3 albedo)
		: m_albedo(albedo)
	{ }
	virtual bool scatter(const Ray& ray, const hit_record& hit, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 target = hit.p + hit.normal + random_in_unit_sphere();
		scattered = Ray(hit.p, target - hit.p);
		attenuation = m_albedo;
		return true;
	}

private:
	Vec3 m_albedo;
};

class Metal : public Material
{
public:
	Metal(Vec3 albedo, float fuzz)
		: m_albedo(albedo)
		, m_fuzz(fuzz)
	{ }
	virtual bool scatter(const Ray& ray, const hit_record& hit, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 reflected = reflect(unit_vector(ray.direction()), hit.normal);
		scattered = Ray(hit.p, reflected + m_fuzz * random_in_unit_sphere());
		attenuation = m_albedo;
		return (dot(scattered.direction(), hit.normal) > 0);
	}

private:
	Vec3 m_albedo;
	float m_fuzz;
};

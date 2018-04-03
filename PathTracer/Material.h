#pragma once

#include "ray.h"
#include "RayTarget.h"

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

	static bool refract(const Vec3& v, const Vec3& n, const float ior, Vec3& refract)
	{
		Vec3 uv = unit_vector(v);
		Vec3 un = unit_vector(n);

		float dt = dot(uv, un);

		float discriminant = 1.f - ior * ior * (1.f - dt * dt);

		if (discriminant > 0.f)
		{
			refract = ior * (uv - un * dt) - n * sqrt(discriminant);
			return true;
		}

		return false;
	}

	static float schlick(const float cosine, const float ior)
	{
		float r0 = (1.f - ior) / (1.f + ior);
		r0 *= r0;
		return (1.f - r0) * powf(1.f - cosine, 5);
	}
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

class Dielectric : public Material
{
public:
	Dielectric(float ior)
		: m_ior(ior)
	{ }

	virtual bool scatter(const Ray& ray, const hit_record& hit, Vec3& attenuation, Ray& scattered) const
	{
		attenuation = Vec3(1.f, 1.f, 1.f);

		Vec3 normal, refracted_dir;
		float ior, cosine, reflect_prob;

		Vec3 reflected_dir = reflect(ray.direction(), hit.normal);

		if (dot(ray.direction(), hit.normal) > 0.f)
		{
			normal = -hit.normal;
			ior = m_ior;
			cosine = ior * dot(ray.direction(), hit.normal) / ray.direction().length();
		}
		else
		{
			normal = hit.normal;
			ior = 1.f / m_ior;
			cosine = -ior * dot(ray.direction(), hit.normal) / ray.direction().length();
		}

		if (refract(ray.direction(), normal, ior, refracted_dir))
		{
			scattered = Ray(hit.p, refracted_dir);
			reflect_prob = schlick(cosine, ior);
		}
		else
		{
			scattered = Ray(hit.p, reflected_dir);
			reflect_prob = 1.0f;
		}

		if (RAND_FLOAT < reflect_prob)
		{
			scattered = Ray(hit.p, reflected_dir);
		}
		else
		{
			scattered = Ray(hit.p, refracted_dir);
		}

		return true;
	}

private:
	float m_ior;
};


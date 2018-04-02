// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "Camera.h"
#include "RayTarget.h"
#include "Sphere.h"
#include "Material.h"

#define MIN_T (0.0001f)

Vec3 color(const Ray& r, RayTargetList* world, int samplenum)
{
	hit_record hit;
	if (world->hit(r, MIN_T, FLT_MAX, hit))
	{
		Ray scattered;
		Vec3 attenuation;
		if (samplenum < 50 && hit.material->scatter(r, hit, attenuation, scattered))
		{
			return attenuation * color(scattered, world, samplenum + 1);
		}
		return Vec3(0.f, 0.f, 0.f);
	}

	Vec3 unit_dir = unit_vector(r.direction());
	float t = 0.5f * (unit_dir.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;

	Camera cam;
	RayTarget * list[5];
	list[0] = new Sphere(Vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(Vec3(0.1f, 0.2f, 0.5f)));
	list[1] = new Sphere(Vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(Vec3(0.8f, 0.8f, 0.f)));
	list[2] = new Sphere(Vec3(1.f, 0.f, -1.f), 0.5f, new Metal(Vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new Sphere(Vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(Vec3(-1.f, 0.f, -1.f), -0.45f, new Dielectric(1.5f));

	RayTargetList * world = new RayTargetList(list, 5);

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col(0.f, 0.f, 0.f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + RAND_FLOAT) / float(nx);
				float v = float(j + RAND_FLOAT) / float(ny);

				Ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}

			col /= float(ns);
			int ir = int(255.99*sqrt(col[0]));
			int ig = int(255.99*sqrt(col[1]));
			int ib = int(255.99*sqrt(col[2]));
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}

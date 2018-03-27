// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "Camera.h"
#include "RayTarget.h"
#include "Sphere.h"

#define RAND_FLOAT ((float)rand()/(float)((unsigned int)RAND_MAX+1))

Vec3 random_in_unit_sphere() 
{
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(RAND_FLOAT, RAND_FLOAT, RAND_FLOAT) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}

Vec3 color(const Ray& r, RayTargetList* world)
{
	hit_record hit;
	if (world->hit(r, 0.f, FLT_MAX, hit))
	{
		Vec3 target = hit.p + hit.normal + random_in_unit_sphere();
		return 0.5f * color(Ray(hit.p, target - hit.p), world);
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
	RayTarget * list[2];
	list[0] = new Sphere(Vec3(0.f, 0.f, -1.f), 0.5f);
	list[1] = new Sphere(Vec3(0.f, -100.5f, -1.f), 100.f);

	RayTargetList * world = new RayTargetList(list, 2);

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

				Vec3 p = r.point_at_param(2.f);
				col += color(r, world);
			}

			col /= float(ns);
			int ir = int(255.99*sqrt(col[0]));
			int ig = int(255.99*sqrt(col[1]));
			int ib = int(255.99*sqrt(col[2]));
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}

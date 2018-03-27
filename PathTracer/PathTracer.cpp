// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "Camera.h"
#include "RayTarget.h"
#include "Sphere.h"

Vec3 color(const Ray& r, RayTargetList* world)
{
	hit_record hit;
	if (world->hit(r, 0.f, FLT_MAX, hit))
	{
		return unit_vector(0.5f * (hit.normal + 1.f));
	}

	Vec3 unit_dir = unit_vector(r.direction());
	float t = 0.5f * (unit_dir.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	int nx = 800;
	int ny = 400;

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
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			
			Ray r = cam.get_ray(u, v);

			Vec3 p = r.point_at_param(2.f);
			Vec3 col = color(r, world);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}

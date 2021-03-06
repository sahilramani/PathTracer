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

RayTargetList * GenerateRandomScene()
{
	int n = 500;

	RayTarget** list = new RayTarget*[n];
	list[0] = new Sphere(Vec3(0.f, -1000.f, -1.f), 1000.f, new Lambertian(Vec3(0.6f, 0.5f, 0.3f)));

	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			Vec3 rand_pos(a + 0.9f* RAND_FLOAT, 0.2f, b + 0.9f * RAND_FLOAT);
			float rand_mat = RAND_FLOAT;

			if ((rand_pos - Vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
			{
				if (rand_mat < 0.8f) // diffuse
				{
					list[i++] = new Sphere(rand_pos, 0.2f, new Lambertian(Vec3(RAND_FLOAT * RAND_FLOAT, RAND_FLOAT * RAND_FLOAT, RAND_FLOAT * RAND_FLOAT)));
				}
				else if (rand_mat < 0.9f) // metal
				{
					list[i++] = new Sphere(rand_pos, 0.2f, new Metal(Vec3(0.5f * (1.f + RAND_FLOAT), 0.5f * (1.f + RAND_FLOAT), 0.5f * (1.f + RAND_FLOAT)), 0.5f * RAND_FLOAT));
				}
				else
				{
					list[i++] = new Sphere(rand_pos, 0.2f, new Dielectric(1.5f * RAND_FLOAT));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0.f, 1.f, 0.f), 1.f, new Dielectric(1.5f));
	list[i++] = new Sphere(Vec3(-4.f, 1.f, 0.f), 1.f, new Lambertian(Vec3(0.4f, 0.3f, 0.2f)));
	list[i++] = new Sphere(Vec3(4.f, 1.f, 0.f), 1.f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new RayTargetList(list, i);
}

int main()
{
	int nx = 1600;
	int ny = 800;
	int ns = 100;

	Vec3 pos = Vec3(-1.f, 2.f, 6.f);
	Vec3 look_at = Vec3(0.f, 0.f, -1.f);
	float focus_distance = (pos - look_at).length();
	float aperture = 0.1f;

	Camera cam(pos, look_at, Vec3(0.f, 1.f, 0.f), 60.f, float(nx) / float(ny), aperture, focus_distance);

	//RayTarget * list[5];
	//list[0] = new Sphere(Vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(Vec3(0.1f, 0.2f, 0.5f)));
	//list[1] = new Sphere(Vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(Vec3(0.8f, 0.8f, 0.f)));
	//list[2] = new Sphere(Vec3(1.f, 0.f, -1.f), 0.5f, new Metal(Vec3(0.8f, 0.6f, 0.2f), 0.3f));
	//list[3] = new Sphere(Vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f));
	//list[4] = new Sphere(Vec3(-1.f, 0.f, -1.f), -0.45f, new Dielectric(1.5f));

	//RayTargetList * world = new RayTargetList(list, 5);

	RayTargetList* world = GenerateRandomScene();

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

#pragma once

#include "ray.h"

struct hit_record
{
	float t;
	Vec3 p;
	Vec3 normal;
};

class RayTarget
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& hit) const = 0;
};

class RayTargetList : public RayTarget
{
public:
	RayTargetList()
		: m_list(nullptr)
		, m_size(0)
	{}

	RayTargetList(RayTarget** list, int n)
		: m_list(list)
		, m_size(n)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& hit) const;

private:
	RayTarget ** m_list;
	int m_size;
};

bool RayTargetList::hit(const Ray& r, float t_min, float t_max, hit_record& hit) const
{
	if (m_size <= 0 || m_list == nullptr)
		return false;

	bool any_hit = false;
	hit_record temp_hit;
	float closest_t = t_max;

	for (int i = 0; i < m_size; i++)
	{
		if (m_list[i]->hit(r, t_min, closest_t, temp_hit))
		{
			any_hit = true;
			closest_t = temp_hit.t;
			hit = temp_hit;
		}
	}
	return any_hit;
}
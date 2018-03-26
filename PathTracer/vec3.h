#pragma once

#include <cmath>
#include <sstream>
#include <cassert>

class Vec3
{
public:
	Vec3() 
	{
		m_val[0] = m_val[1] = m_val[2] = 0;
	}

	Vec3(float x, float y, float z) 
	{
		m_val[0] = x;
		m_val[1] = y;
		m_val[2] = z;
	}

	float x() const { return m_val[0]; }
	float y() const { return m_val[1]; }
	float z() const { return m_val[2]; }

	float r() const { return m_val[0]; }
	float g() const { return m_val[1]; }
	float b() const { return m_val[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-m_val[0], -m_val[1], -m_val[2]); }
	inline float operator[](int i) const { return m_val[i]; }
	inline float& operator[](int i) { return m_val[i]; }

	inline Vec3& operator+=(const Vec3& other);
	inline Vec3& operator-=(const Vec3& other);
	inline Vec3& operator*=(const Vec3& other);
	inline Vec3& operator/=(const Vec3& other);
	inline Vec3& operator+=(float other);
	inline Vec3& operator-=(float other);
	inline Vec3& operator*=(float other);
	inline Vec3& operator/=(float other);

	inline float length() const
	{
		return sqrt(squared_length());
	}

	inline float squared_length() const
	{
		assert(!(m_val[0] == 0 && m_val[1] == 0 && m_val[2] == 0));
		return (sqrt(m_val[0] * m_val[0] + m_val[1] * m_val[1] + m_val[2] * m_val[2]));
	}

	inline void make_unit_vector();

private:
	float m_val[3];
};

#pragma region Class Implementation

inline Vec3& Vec3::operator+=(const Vec3& other)
{
	m_val[0] += other.m_val[0];
	m_val[1] += other.m_val[1];
	m_val[2] += other.m_val[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& other)
{
	m_val[0] -= other.m_val[0];
	m_val[1] -= other.m_val[1];
	m_val[2] -= other.m_val[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& other)
{
	m_val[0] *= other.m_val[0];
	m_val[1] *= other.m_val[1];
	m_val[2] *= other.m_val[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& other)
{
	m_val[0] /= other.m_val[0];
	m_val[1] /= other.m_val[1];
	m_val[2] /= other.m_val[2];
	return *this;
}

inline Vec3& Vec3::operator+=(const float other)
{
	m_val[0] += other;
	m_val[1] += other;
	m_val[2] += other;
	return *this;
}

inline Vec3& Vec3::operator-=(const float other)
{
	m_val[0] -= other;
	m_val[1] -= other;
	m_val[2] -= other;
	return *this;
}

inline Vec3& Vec3::operator*=(const float other)
{
	m_val[0] *= other;
	m_val[1] *= other;
	m_val[2] *= other;
	return *this;
}

inline Vec3& Vec3::operator/=(const float other)
{
	m_val[0] /= other;
	m_val[1] /= other;
	m_val[2] /= other;
	return *this;
}

inline void Vec3::make_unit_vector()
{
	assert(!(m_val[0] == 0 && m_val[1] == 0 && m_val[2] == 0));
	float k = 1.0f / squared_length();
	m_val[0] *= k;
	m_val[1] *= k;
	m_val[2] *= k;
}

#pragma endregion Class Implementation

#pragma region Helper Functions

inline std::istream& operator>>(std::istream& istream, Vec3 &val)
{
	istream >> val[0] >> val[1] >> val[2];
	return istream;
}

inline std::ostream& operator<<(std::ostream& ostream, const Vec3 &val)
{
	ostream << val[0] << val[1] << val[2];
	return ostream;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline Vec3 operator+(const Vec3& v, float val)
{
	return Vec3(v[0] + val, v[1] + val, v[2] + val);
}

inline Vec3 operator-(const Vec3& v, float val)
{
	return Vec3(v[0] - val, v[1] - val, v[2] - val);
}

inline Vec3 operator*(const Vec3& v, float val)
{
	return Vec3(v[0] * val, v[1] * val, v[2] * val);
}

inline Vec3 operator/(const Vec3& v, float val)
{
	return Vec3(v[0] / val, v[1] / val, v[2] / val);
}

inline Vec3 operator*(float val, const Vec3& v)
{
	return Vec3(val * v[0], val * v[1], val * v[2]);
}

inline float dot(const Vec3& v1, const Vec3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3(
		v1[1] * v2[2] - v1[2] * v2[1],
		-(v1[0] * v2[2] - v1[2] * v2[0]),
		v1[0] * v2[1] - v1[1] * v2[0]
	);
}

inline Vec3 unit_vector(const Vec3& v)
{
	return v / v.length();
}

#pragma endregion Helper Functions

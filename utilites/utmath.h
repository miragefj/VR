#pragma once
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <float.h> //FLT_EPSILON, FLT_PI
#include "glmath.h"

#ifdef GAVNO
enum {
	X, Y, Z, W
};

#ifndef min
#define min(a, b) (a < b) ? a : b
#endif

#ifndef max
#define max(a, b) (a > b) ? a : b
#endif

#ifndef abs
#define abs(f) (f < 0.f) ? -f : f
#endif

//https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
float clampf(float d, float min, float max)
{
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

class CVector2
{
public:
	CVector2() {}
	CVector2(float xx, float yy, float zz) : x(xx), y(yy) {}

	CVector2 &operator+(const CVector2 &vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	float x, y;
};

class CVector3
{
public:
	CVector3() {}
	CVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	CVector3(float vv[3]) : x(vv[0]), y(vv[1]), z(vv[2]) {}
	CVector3(float y, float p) : x( -sin(y) * cos(p) ), y( cos(y) * cos(p) ), z( sin(p) ) {}
	~CVector3() {}
	__forceinline bool IsZero() { return x == 0.f && y == 0.f && z == 0.f; }
	__forceinline void SetZero() { x = 0.f, y = 0.f, z = 0.f; }
	__forceinline float SquaredLen() const { return x * x + y * y + z * z; }
	__forceinline float DotProduct(CVector3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; } //��������� ������������ ��������
	
	//������������ ������������ ��������
	//TODO: ���������, �������� ������� ��������
	__forceinline CVector3 &CrossProduct(CVector3 &vec) {
		x = (y * vec.z - z * vec.y);
		y = -(x * vec.z - z * vec.x);
		z = (x * vec.y - y * vec.x);
		return *this;
	}
	CVector3 &Min(CVector3 &v) { x = min(x, v.x); y = min(y, v.y); z = min(y, v.y); return *this; }
	CVector3 &Max(CVector3 &v) { x = max(x, v.x); y = max(y, v.y); z = max(y, v.y); return *this; }
	CVector3 &Min(float f) { x = min(x, f); y = min(y, f); z = min(y, f); return *this; }
	CVector3 &Max(float f) { x = max(x, f); y = max(y, f); z = max(y, f); return *this; }
	CVector3 &Clamp(float min, float max) { x = clampf(x, min, max); y = clampf(y, min, max); z = clampf(z, min, max); return *this; }
	void Negative() { x = -x; y = -y; z = -z; }
	float Magnitude() const { return sqrtf(SquaredLen()); }
	CVector3 &Scale(float k) { x *= k; y *= k; z *= k; return *this; }

	//TODO: check epsilon in normalization
	CVector3 &Normalize() {
		float mag = Magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
		return *this;
	}
	bool IsNormalized() { float l = SquaredLen(); return (l > 0.99f && l < 1.01f); }
	float Distance(CVector3 &v, CVector3 &dest) { dest = *this; dest - v; return dest.Magnitude(); }

	//TODO: check linear interpolation
	CVector3 &Lerp(CVector3 &v, float t) {
		x += (v.x - x) * t;
		y += (v.y - y) * t;
		z += (v.z - z) * t;
		return *this;
	}

	CVector3 &operator+(const CVector3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; } //add vector
	CVector3 &operator+=(const CVector3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; } //add vector
	CVector3 &operator-(const CVector3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; } //sub vector
	CVector3 &operator-=(const CVector3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; } //sub vector
	CVector3 &operator*(const CVector3 &vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; } //mul vector
	CVector3 &operator/(const CVector3 &vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this; } //div vector
	bool operator==(CVector3 vec) { return x == vec.x && y == vec.y && z == vec.z; }
	bool operator!=(CVector3 vec) { return x != vec.x && y != vec.y && z != vec.z; }

	union {
		struct { float x, y, z; };
		float v[3];
	};
};

CVector3 cross(const CVector3 &u, const CVector3 &v)
{
	return CVector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float dot(const CVector3 &u, const CVector3 &v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float length(const CVector3 &u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

float length2(const CVector3 &u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}
#endif

//CVector3 mix(const CVector3 &u, const CVector3 &v, float a)
//{
//	return u * (1.0f - a) + v * a;
//}
//
//CVector3 normalize(const CVector3 &u)
//{
//	return u / sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
//}
//
//CVector3 reflect(const CVector3 &i, const CVector3 &n)
//{
//	return i - 2.0f * dot(n, i) * n;
//}
//
//CVector3 refract(const CVector3 &i, const CVector3 &n, float eta)
//{
//	CVector3 r;
//	float ndoti = dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);
//	if (k >= 0.0f)
//	{
//		r = eta * i - n * (eta * ndoti + sqrt(k));
//	}
//	return r;
//}
//
//CVector3 rotate(const CVector3 &u, float angle, const CVector3 &v)
//{
//	return *(CVector3*)&(rotate(angle, v) * vec4(u, 1.0f));
//}

static void Draw3DSGrid()
{
	// Turn the lines GREEN
	glColor3ub(0, 255, 0);
	int size = 500;
	// Draw a 1x1 grid along the X and Z axis'
	for (float i = -size; i <= size; i++)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

		// Do the horizontal lines (along the X)
		glVertex3f(-size, 0, i);
		glVertex3f(size, 0, i);

		// Do the vertical lines (along the Z)
		glVertex3f(i, 0, -size);
		glVertex3f(i, 0, size);

		// Stop drawing lines
		glEnd();
	}
}

static void DrawAxis()
{
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glColor3ub(0, 255, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glColor3ub(0, 0, 255);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();
	glPopAttrib();
}

static void DrawBBox(vec3 min, vec3 max)
{
	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);

	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();
}

static void DrawBBox(vec3int min, vec3int max)
{
	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);

	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();
}

static float last_time = 0;

static int FrameRate()
{
	//static float last_time = GetTickCount() * 0.001f;
	float current_time = (float)GetTickCount()/* * 0.001f*/;
	
	float deltaTime = fabs(current_time - last_time);
	last_time = current_time;
	int fps = (int)1.0f / deltaTime;
	return fps;
}

//�����������
//3d plane at 4 points
class CRect3
{
public:
	CRect3() {}
	CRect3(vec3 start, float width) { InitByStartPoint(start, width); }
	CRect3(vec3 start, vec3 end) { InitByVectors(start, end); }

	void InitByStartPoint(vec3 start, float width)
	{
		p1 = start;
		p2.x = start.x + width;
		p2.y = start.y;
		p2.z = start.z;
		p3.x = start.x + width;
		p3.y = start.y;
		p3.z = start.z + width;
		p4.x = start.x;
		p4.y = start.y;
		p4.z = start.z + width;
	}

	void InitByVectors(vec3 &start, vec3 &end) {
		/*
				 (2)-----------------(3)
				   \                   \
					\                   \
					 \                   \
					 (1)-----------------(4)
		*/
		float width = fabs(end.z - start.z);
		float length = fabs(end.x - start.x);
		p1 = start;
		p2.x = start.x + width;
		p2.y = start.y;
		p2.z = start.z;

		p3.x = start.x + width;
		p3.y = start.y;
		p3.z = start.z + length;

		p4.x = start.x;
		p4.y = start.y;
		p4.z = start.z + length;
	}
	~CRect3() {}

	vec3 p1, p2, p3, p4;
};

class CTriangle
{
public:
	CTriangle() {}
	CTriangle(vec3 pp1, vec3 pp2, vec3 pp3) : p1(pp1), p2(pp2), p3(pp3) {};
	~CTriangle() {}

	vec3 p1, p2, p3;
};

class CPlane3
{
public:
	CPlane3() {}
	CPlane3(vec3 &origin, vec3 &normal) {
		m_origin = origin;
		m_normal = normal;
	}
	CPlane3(float originx, float originy, float originz, float normx, float normy, float normz) : m_origin(vec3(originx, originy, originz)), m_normal(vec3(normx, normy, normz)) {}
	~CPlane3() {}

	void SetOrigin(vec3 &origin) { m_origin = origin; }
	void SetOrigin(float x, float y, float z) { m_origin = vec3(x, y, z); }
	void SetNormal(vec3 &normal) { m_normal = normal; }
	void SetNormal(float nx, float ny, float nz) { m_normal = vec3(nx, ny, nz); }

	vec3 m_origin;
	vec3 m_normal;
};

//ray
class CRay
{
public:
	CRay() {}
	CRay(vec3 sstart, vec3 ddir) : m_origin(sstart), m_direction(ddir) {}
	CRay(float xorigin, float yorigin, float zorigin, float xdir, float ydir, float zdir) : m_origin(vec3(xorigin, yorigin, zorigin)), m_direction(vec3(xdir, ydir, zdir)) {}
	~CRay() {}

	void SetOrigin(vec3 origin) { m_origin = origin; }
	void SetOrigin(float x, float y, float z) { m_origin = vec3(x, y, z); }
	void SetDirection(vec3 dir) { m_direction = dir; }
	void SetDirection(float x, float y, float z) { m_direction = vec3(x, y, z); }
	void SetLength(float len) { m_length = len; }

	int TriangleIntersection(vec3 &v0, vec3 &v1, vec3 &v2, vec3 &intersection) {
		vec3 e1 = v1 - v0;
		vec3 e2 = v2 - v0;
		// ���������� ������� ������� � ���������
		vec3 pvec = cross(m_direction, e2);
		float det = dot(e1, pvec);

		// ��� ���������� ���������
		if (det < 1e-8 && det > -1e-8) {
			return 0;
		}

		float inv_det = 1 / det;
		vec3 tvec = m_origin - v0;
		float u = dot(tvec, pvec) * inv_det;
		if (u < 0 || u > 1) {
			return 0;
		}

		vec3 qvec = cross(tvec, e1);
		float v = dot(m_direction, qvec) * inv_det;
		if (v < 0 || u + v > 1) {
			return 0;
		}
		
		float t = dot(e2, qvec) * inv_det;
		intersection = m_origin + m_direction * t;
		return 1;
	}

	//https://stackoverflow.com/questions/23975555/how-to-do-ray-plane-intersection
	int PlaneIntersection(CPlane3 &plane, vec3 &intersectionpoint, float IntersectionPointRoundFactor, float IntersectionPlaneD) {
		float NdotR = -dot(plane.m_normal, m_direction * IntersectionPointRoundFactor);
		//float NdotR = -dot(plane.m_normal, m_origin * IntersectionPointRoundFactor);
		if (NdotR != 0.0f) {
			float Distance = (dot(plane.m_normal, m_origin) + IntersectionPlaneD) / NdotR;
			if (Distance > 0.125f) {
				intersectionpoint = m_origin + m_direction * Distance;
				//intersectionpoint.x = round(intersectionpoint.x / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;
				//intersectionpoint.y = round(intersectionpoint.y / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;
				//intersectionpoint.z = round(intersectionpoint.z / IntersectionPointRoundFactor) * IntersectionPointRoundFactor;
				return 1;
			}
		}
		return 0;
	}

	// point = origin + direction * t
	vec3 Evaluate(float t) { return m_origin + m_direction * t; }

	// Source Function Plane Intersection Test
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	//where l0 is the origin of the ray and l is the ray direction.
	//and the plane normal - n.
	//t - distance
	/*
	bool intersectPlane(const Vec3f &n, const Vec3f &p0, const Vec3f &l0, const Vec3f &l, float &t)
	{
		// assuming vectors are all normalized
		float denom = dotProduct(n, l);
		if (denom > 1e-6) {
			Vec3f p0l0 = p0 - l0;
			t = dotProduct(p0l0, n) / denom;
			return (t >= 0);
		}

		return false;
	}
	*/
	int PlaneIntersection2(CPlane3 &plane, vec3 &intersectionpoint)
	{
		// assuming vectors are all normalized
		float denom = -dot(plane.m_normal, m_direction);
		if (denom > 1e-6) {
			vec3 p0l0 = plane.m_origin - m_origin;
			float t = -dot(p0l0, plane.m_normal) / denom;
			if (t >= 0) {
				intersectionpoint = m_origin + m_direction * t;//���������
				//intersectionpoint = ray_evaluate(*this, t);
				return 1;
			}
		}
		return 0;
	}

	int PlaneIntersection3(CPlane3 &plane, vec3 &intersectionpoint)
	{
		mat3x3 a(m_direction.y, 0.f, plane.m_normal.x, -m_direction.x, m_direction.z, plane.m_normal.y, 0.f, -m_direction.y, plane.m_normal.z);
		vec3 b(
			m_direction.y * m_origin.x - m_direction.x * m_origin.y,
			m_direction.z * m_origin.y - m_direction.y * m_origin.z,
			dot(plane.m_normal, plane.m_origin)
		);
		intersectionpoint = inverse(a) * b;
		return 1;
	}

	//http://mathprofi.ru/zadachi_s_pryamoi_i_ploskostju.html
	//PlaneIntersection4 == PlaneIntersection5
	int PlaneIntersection4(CPlane3 &plane, vec3 &intersectionpoint)
	{
		float t = (dot(plane.m_normal, plane.m_origin) - dot(plane.m_normal, m_origin)) / dot(plane.m_normal, m_direction);
		intersectionpoint = m_origin + m_direction * t;
		return 1;
	}

	int PlaneIntersection5(vec3 planeP, vec3 planeN, vec3 rayP, vec3 rayD, vec3 &intersect)
	{
		float d = dot(planeP, -planeN);
		float t = -(d + rayP.z * planeN.z + rayP.y * planeN.y + rayP.x * planeN.x) / (rayD.z * planeN.z + rayD.y * planeN.y + rayD.x * planeN.x);
		if (t > 0.f) {
			//printf("d = %f\n", d);
			intersect = rayP + t * rayD;
			return 1;
		}
		return 0;
	}

	// p,v - ray
	// n,d - plane
	int PlaneIntersection6(vec3 p, vec3 v, vec3 n, float d, vec3 &intersect) {
		float denom = dot(n, v);
		// Prevent divide by zero:
		if (fabs(denom) <= 1e-4f)
			return 0;

		// If you want to ensure the ray reflects off only
		// the "top" half of the plane, use this instead:
		if (-denom <= 1e-4f)
			return 0;

		float t = -(dot(n, p) + d) / dot(n, v);

		// Use pointy end of the ray.
		// It is technically correct to compare t < 0,
		// but that may be undesirable in a raytracer.
		if (t <= 1e-4)
			return 0;

		intersect = p + t * v;
		return 1;
	}

	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	/*
	bool intersectDisk(const Vec3f &n, const Vec3f &p0, const float &radius, const Vec3f &l0, const Vec3 &l)
	{
		float t = 0;
		if (intersectPlane(n, p0, l0, l, t)) {
			Vec3f p = l0 + l * t;
			Vec3f v = p - p0;
			float d2 = dot(v, v);
			return (sqrtf(d2) <= radius);
			// or you can use the following optimisation (and precompute radius^2)
			// return d2 <= radius2; // where radius2 = radius * radius
		 }

		 return false;
	}
	*/

	vec3 m_origin;
	vec3 m_direction;
	float m_length;
};

class CAABB
{
public:
	CAABB() {}
	CAABB(vec3 vmin, vec3 vmax) : Min(vmin), Max(vmax) {}
	CAABB(float x, float y, float z, float size) {
		Min = vec3(x, y, z);
		Max = vec3(x + size, y + size, z + size);
	}
	~CAABB() {}

	void Draw() { DrawBBox(Min, Max); }
	bool PointInside(vec3 &point) { return Min.x < point.x && Min.y < point.y && Min.z < point.z && Max.x > point.x && Max.y > point.y && Max.z > point.z; }
	bool BboxInside(CAABB &bbox) { return Min < bbox.Min && Max > bbox.Max; }

	bool RayIntersect(CRay &ray, float *ptmin, float *ptmax) {
		double tmin = -INFINITY, tmax = INFINITY;
		for (int i = 0; i < 3; ++i) {
			if (ray.m_direction[i] != 0.0) {
				double t1 = (Min[i] - ray.m_origin[i]) / ray.m_direction[i];
				double t2 = (Max[i] - ray.m_origin[i]) / ray.m_direction[i];

				tmin = max(tmin, min(t1, t2));
				tmax = min(tmax, max(t1, t2));
			}
			else if (ray.m_origin[i] < Min[i] || ray.m_origin[i] > Max[i])
				return false;
		}

		if (ptmin)
			*ptmin = tmin;

		if (ptmax)
			*ptmax = tmax;

		return tmax >= tmin && tmax >= 0.0;
	}

	vec3 Min, Max;
};

//https://tavianator.com/2011/ray_box.html
//https://tavianator.com/2015/ray_box_nan.html
//�� ���������
static bool aabb_intersection1(CAABB &b, CRay &r) {
	double t1, t2, tmin = -INFINITY, tmax = INFINITY;
	vec3 dir_inv = r.m_direction.inverse();
	t1 = (b.Min.x - r.m_origin.x) * dir_inv.x;
	t2 = (b.Max.x - r.m_origin.x) * dir_inv.x;
	tmin = max(tmin, min(t1, t2));
	tmax = min(tmax, max(t1, t2));

	t1 = (b.Min.y - r.m_origin.y) * dir_inv.y;
	t2 = (b.Max.y - r.m_origin.y) * dir_inv.y;
	tmin = max(tmin, min(t1, t2));
	tmax = min(tmax, max(t1, t2));

	t1 = (b.Min.z - r.m_origin.z) * dir_inv.z;
	t2 = (b.Max.z - r.m_origin.z) * dir_inv.z;
	tmin = max(tmin, min(t1, t2));
	tmax = min(tmax, max(t1, t2));

	return tmax > max(tmin, 0.0);
}

//OK
static bool aabb_intersection2(CAABB &b, CRay &ray, float *ptmin, float *ptmax) {
	double tmin = -INFINITY, tmax = INFINITY;
	for (int i = 0; i < 3; ++i) {
		if (ray.m_direction[i] != 0.0) {
			double t1 = (b.Min[i] - ray.m_origin[i]) / ray.m_direction[i];
			double t2 = (b.Max[i] - ray.m_origin[i]) / ray.m_direction[i];

			tmin = max(tmin, min(t1, t2));
			tmax = min(tmax, max(t1, t2));
		}
		else if (ray.m_origin[i] < b.Min[i] || ray.m_origin[i] > b.Max[i])
			return false;
	}
	*ptmin = tmin;
	*ptmax = tmax;
	return tmax >= tmin && tmax >= 0.0;
}
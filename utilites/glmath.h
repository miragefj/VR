#pragma once
// ----------------------------------------------------------------------------------------------------------------------------
// Updated: 03.09.2021
// Version 4.00
// Original parts from: http://www.3dcpptutorials.sk/
// 
// Changelog:
// -03.09.2021 - added basic vectors functions, quaternions class
// 
// 
// ----------------------------------------------------------------------------------------------------------------------------
#include <math.h>
#include <assert.h>

#define PI 3.141592653589793238462643
#define DTOR            0.0174532925    
#define RTOD            57.2957795   

#define __mmin(a, b) (a < b) ? a : b
#define __mmax(a, b) (a > b) ? a : b
#define __mabs(f) (f < 0.f) ? -f : f

// ----------------------------------------------------------------------------------------------------------------------------
class vec2
{
public:
	vec2() : x(0.0f), y(0.0f){}
	~vec2(){}
	vec2(float num) : x(num), y(num){}
	vec2(float x, float y) : x(x), y(y){}
	vec2(const vec2 &u) : x(u.x), y(u.y){}
	vec2& operator=(const vec2 &u){x = u.x; y = u.y; return *this;}
	vec2 operator-(){return vec2(-x, -y);}
	vec2 &operator+=(float num){x += num; y += num; return *this;}
	vec2 &operator+=(const vec2 &u){x += u.x; y += u.y; return *this;}
	vec2 &operator-=(float num){x -= num; y -= num; return *this;}
	vec2 &operator-=(const vec2 &u){x -= u.x; y -= u.y; return *this;}
	vec2 &operator*=(float num){x *= num; y *= num; return *this;}
	vec2 &operator*=(const vec2 &u){x *= u.x; y *= u.y; return *this;}
	vec2 &operator/=(float num){x /= num; y /= num; return *this;}
	vec2 &operator/=(const vec2 &u){x /= u.x; y /= u.y; return *this;}
	friend vec2 operator+(const vec2 &u, float num){return vec2(u.x + num, u.y + num);}
	friend vec2 operator+(float num, const vec2 &u){return vec2(num + u.x, num + u.y);}
	friend vec2 operator+(const vec2 &u, vec2 &v){return vec2(u.x + v.x, u.y + v.y);}
	friend vec2 operator-(const vec2 &u, float num){return vec2(u.x - num, u.y - num);}
	friend vec2 operator-(float num, const vec2 &u){return vec2(num - u.x, num - u.y);}
	friend vec2 operator-(const vec2 &u, const vec2 &v){return vec2(u.x - v.x, u.y - v.y);}
	friend vec2 operator*(const vec2 &u, float num){return vec2(u.x * num, u.y * num);}
	friend vec2 operator*(float num, const vec2 &u){return vec2(num * u.x, num * u.y);}
	friend vec2 operator*(const vec2 &u, const vec2 &v){return vec2(u.x * v.x, u.y * v.y);}
	friend vec2 operator/(const vec2 &u, float num){return vec2(u.x / num, u.y / num);}
	friend vec2 operator/(float num, const vec2 &u){return vec2(num / u.x, num / u.y);}
	friend vec2 operator/(const vec2 &u, const vec2 &v){return vec2(u.x / v.x, u.y / v.y);}
	union {
		struct { float x, y; };
		struct { float s, t; };
		//struct { float u, v; };
		struct { float arr[2]; };
	};
};

// ----------------------------------------------------------------------------------------------------------------------------
float dot(const vec2 &u, const vec2 &v);
float length(const vec2 &u);
float length2(const vec2 &u);
//vec2 mix(const vec2 &u, const vec2 &v, float a);
vec2 normalize(const vec2 &u);
vec2 reflect(const vec2 &i, const vec2 &n);
vec2 refract(const vec2 &i, const vec2 &n, float eta);
vec2 rotate(const vec2 &u, float angle);
// ----------------------------------------------------------------------------------------------------------------------------

class vec3
{
public:
	vec3() : x(0.0f), y(0.0f), z(0.0f){}
	~vec3(){}
	vec3(float num) : x(num), y(num), z(num){}
	vec3(float x, float y, float z) : x(x), y(y), z(z){}
	vec3(const vec2 &u, float z) : x(u.x), y(u.y), z(z){}
	vec3(const vec3 &u) : x(u.x), y(u.y), z(u.z){}
	vec3(int xx, int yy, int zz) : x((float)xx), y((float)yy), z((float)zz){}
	vec3& operator = (const vec3 &u){x = u.x; y = u.y; z = u.z; return *this;}
	vec3 operator - (){return vec3(-x, -y, -z);}
	vec3& operator += (float num){x += num; y += num; z += num; return *this;}
	vec3& operator += (const vec3 &u){x += u.x; y += u.y; z += u.z; return *this;}
	vec3& operator -= (float num){x -= num; y -= num; z -= num; return *this;}
	vec3& operator -= (const vec3 &u){x -= u.x; y -= u.y; z -= u.z; return *this;}
	vec3& operator *= (float num){x *= num; y *= num; z *= num; return *this;}
	vec3& operator *= (const vec3 &u){x *= u.x; y *= u.y; z *= u.z; return *this;}
	vec3& operator /= (float num){x /= num; y /= num; z /= num; return *this;}
	vec3& operator /= (const vec3 &u){x /= u.x; y /= u.y; z /= u.z; return *this;}
	friend vec3 operator + (const vec3 &u, float num){return vec3(u.x + num, u.y + num, u.z + num);}
	friend vec3 operator + (float num, const vec3 &u){return vec3(num + u.x, num + u.y, num + u.z);}
	friend vec3 operator + (const vec3 &u, const vec3 &v){return vec3(u.x + v.x, u.y + v.y, u.z + v.z);}
	friend vec3 operator - (const vec3 &u, float num){return vec3(u.x - num, u.y - num, u.z - num);}
	friend vec3 operator - (float num, const vec3 &u){return vec3(num - u.x, num - u.y, num - u.z);}
	friend vec3 operator - (const vec3 &u, const vec3 &v){return vec3(u.x - v.x, u.y - v.y, u.z - v.z);}
	friend vec3 operator * (const vec3 &u, float num){return vec3(u.x * num, u.y * num, u.z * num);}
	friend vec3 operator * (float num, const vec3 &u){return vec3(num * u.x, num * u.y, num * u.z);}
	friend vec3 operator * (const vec3 &u, const vec3 &v){return vec3(u.x * v.x, u.y * v.y, u.z * v.z);}
	friend vec3 operator / (const vec3 &u, float num){return vec3(u.x / num, u.y / num, u.z / num);}
	friend vec3 operator / (float num, const vec3 &u){return vec3(num / u.x, num / u.y, num / u.z);}
	friend vec3 operator / (const vec3 &u, const vec3 &v){return vec3(u.x / v.x, u.y / v.y, u.z / v.z);}

	inline bool operator == (vec3 &v) { return x == v.x && y == v.y && z == v.z; }
	inline bool operator < (vec3 &v) { return x < v.x && y < v.y && z < v.z; }
	inline bool operator <= (vec3 &v) { return x <= v.x && y <= v.y && z <= v.z; }
	inline bool operator > (vec3 &v) { return x > v.x && y > v.y && z > v.z; }
	inline bool operator >= (vec3 &v) { return x >= v.x && y >= v.y && z >= v.z; }
	inline void operator ()(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	inline void operator ()(vec3 v) { x = v.x; y = v.y; z = v.z; }
	inline float operator[](int i) { return v[i]; }

	vec3 inverse() { return vec3(-x, -y, -z); }

	union {
		struct { float x, y, z; };
		struct { float s, t, p; };
		struct { float r, g, b; };
		struct { float v[3]; };
	};
};

// ----------------------------------------------------------------------------------------------------------------------------
vec3 add(vec3 a, vec3 b);
vec3 sub(vec3 a, vec3 b);
vec3 mul(vec3 a, vec3 b);
vec3 mulsc(vec3 a, float scalar);

vec3 cross(const vec3 &u, const vec3 &v);
float dot(const vec3 &u, const vec3 &v);
float length(const vec3 &u);
float length_squared(const vec3 &u);
vec3 mix(const vec3 &u, const vec3 &v, float a);
vec3 normalize(const vec3 &u);
vec3 reflect(const vec3 &i, const vec3 &n);
vec3 refract(const vec3 &i, const vec3 &n, float eta);
vec3 rotate(const vec3 &u, float angle, const vec3 &v);
vec3 vecabs(vec3 &v);

float distance(float ax, float ay, float az, float bx, float by, float bz);
float distance(vec3 &a, vec3 &b);

vec3 Min(const vec3 &a, const vec3 &b);
vec3 Max(const vec3 &a, const vec3 &b);

void round_vector(vec3 &dest, const vec3 vec, float roundfactor);
// ----------------------------------------------------------------------------------------------------------------------------

class vec3int
{
public:
	vec3int() : x(0), y(0), z(0) {}
	vec3int(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
	vec3int(vec3 vecf) : x((int)vecf.x), y((int)vecf.y), z((int)vecf.z) {}
	~vec3int() {}
	union {
		struct { int x, y, z; };
		struct { int i, j, k; };
	};
};
// ----------------------------------------------------------------------------------------------------------------------------

class vec4
{
public:
	vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f){}
	~vec4(){}
	vec4(float num) : x(num), y(num), z(num), w(num){}
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
	vec4(const vec2 &u, float z, float w) : x(u.x), y(u.y), z(z), w(w){}
	vec4(const vec3 &u, float w) : x(u.x), y(u.y), z(u.z), w(w){}
	vec4(const vec4 &u) : x(u.x), y(u.y), z(u.z), w(u.w){}
	vec4& operator = (const vec4 &u){x = u.x; y = u.y; z = u.z; w = u.w; return *this;}
	vec4 operator - (){return vec4(-x, -y, -z, -w);}
	float* operator & (){return (float*)this;}
	vec4& operator += (float num){x += num; y += num; z += num; w += num; return *this;}
	vec4& operator += (const vec4 &u){x += u.x; y += u.y; z += u.z; w += u.w; return *this;}
	vec4& operator -= (float num){x -= num; y -= num; z -= num; w -= num; return *this;}
	vec4& operator -= (const vec4 &u){x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this;}
	vec4& operator *= (float num){x *= num; y *= num; z *= num; w *= num; return *this;}
	vec4& operator *= (const vec4 &u){x *= u.x; y *= u.y; z *= u.z; w *= u.w; return *this;}
	vec4& operator /= (float num){x /= num; y /= num; z /= num; w /= num; return *this;}
	vec4& operator /= (const vec4 &u){x /= u.x; y /= u.y; z /= u.z; w /= u.w; return *this;}
	friend vec4 operator + (const vec4 &u, float num){return vec4(u.x + num, u.y + num, u.z + num, u.w + num);}
	friend vec4 operator + (float num, const vec4 &u){return vec4(num + u.x, num + u.y, num + u.z, num + u.w);}
	friend vec4 operator + (const vec4 &u, const vec4 &v){return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);}
	friend vec4 operator - (const vec4 &u, float num){return vec4(u.x - num, u.y - num, u.z - num, u.w - num);}
	friend vec4 operator - (float num, const vec4 &u){return vec4(num - u.x, num - u.y, num - u.z, num - u.w);}
	friend vec4 operator - (const vec4 &u, const vec4 &v){return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);}
	friend vec4 operator * (const vec4 &u, float num){return vec4(u.x * num, u.y * num, u.z * num, u.w * num);}
	friend vec4 operator * (float num, const vec4 &u){return vec4(num * u.x, num * u.y, num * u.z, num * u.w);}
	friend vec4 operator * (const vec4 &u, const vec4 &v){return vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);}
	friend vec4 operator / (const vec4 &u, float num){return vec4(u.x / num, u.y / num, u.z / num, u.w / num);}
	friend vec4 operator / (float num, const vec4 &u){return vec4(num / u.x, num / u.y, num / u.z, num / u.w);}
	friend vec4 operator / (const vec4 &u, const vec4 &v){return vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);}

	union {
		struct { float x, y, z, w; };
		struct { float s, t, p, q; };
		struct { float r, g, b, a; };
	};
};

class quat
{
public:
	quat() : x(0.f), y(0.f), z(0.f), w(0.f) {}
	quat(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}
	quat(vec3 vec, float ww) : x(vec.x), y(vec.y), z(vec.z), w(ww) {}
	~quat() {}

	bool operator==(quat q) { return x == q.x && y == q.y && z == q.z && w == q.w; }
	bool operator!=(quat q) { return x != q.x && y != q.y && z != q.z && w != q.w; }
	vec3 vec() { return vec3(x, y, z); }

	float x, y, z, w;
};

quat computew(quat q);
float length_squared(quat q);
float length(quat q);
float dot(quat qa, quat qb);
quat normalize(quat q);
quat mul(quat qa, quat qb);
quat mul(quat qa, vec3 vb);
quat slerp(quat &qa, quat &qb, float t);
quat conjugate(quat &q);
vec3 rotate_with_quat(vec3 a, quat q);
quat quat_from_angle_axis(float angle, vec3 axis);

// ----------------------------------------------------------------------------------------------------------------------------

class mat2x2;
class mat3x3;
class mat4x4;

// ----------------------------------------------------------------------------------------------------------------------------

class mat2x2
{
public:
	union {
		struct { float M[4]; };
		struct { float m[2][2]; };
	};
	
	mat2x2();
	~mat2x2();
	mat2x2(const mat2x2 &Matrix);
	mat2x2(const vec2 &col1, const vec2 &col2);
	mat2x2(float c1r1, float c1r2, float c2r1, float c2r2);
	explicit mat2x2(const mat3x3 &Matrix);
	explicit mat2x2(const mat4x4 &Matrix);
	mat2x2& operator = (const mat2x2 &Matrix);
	float& operator [] (int i);
	float* operator & ();
	friend mat2x2 operator * (const mat2x2 &Matrix1, const mat2x2 &Matrix2);
	friend vec2 operator * (const mat2x2 &Matrix, const vec2 &u);
};

// ----------------------------------------------------------------------------------------------------------------------------

mat2x2 inverse(const mat2x2 &Matrix);
mat2x2 transpose(const mat2x2 &Matrix);

// ----------------------------------------------------------------------------------------------------------------------------

class mat3x3
{
public:
	union {
		struct { float M[9]; };
		struct { float m[3][3]; };
	};
	mat3x3();
	~mat3x3();
	mat3x3(const mat3x3 &Matrix);
	mat3x3(const vec3 &col1, const vec3 &col2, const vec3 &col3);
	mat3x3(float c1r1, float c1r2, float c1r3, float c2r1, float c2r2, float c2r3, float c3r1, float c3r2, float c3r3);
	explicit mat3x3(const mat2x2 &Matrix);
	explicit mat3x3(const mat4x4 &Matrix);
	mat3x3& operator = (const mat3x3 &Matrix);
	float& operator [] (int i);
	friend mat3x3 operator * (const mat3x3 &Matrix1, const mat3x3 &Matrix2);
	friend vec3 operator * (const mat3x3 &Matrix, const vec3 &u);
};

// ----------------------------------------------------------------------------------------------------------------------------

mat3x3 inverse(const mat3x3 &Matrix);
mat3x3 transpose(const mat3x3 &Matrix);

void EulerAnglesToMatrix3x3(mat3x3 &mat, vec3 &theta);

//// Warning! If Y axis ~== 90.0 deg, then epic fail :) 
////https://stackoverflow.com/questions/15022630/how-to-calculate-the-angle-from-rotation-matrix
//		 | r11 r12 r13 |
//	 R = | r21 r22 r23 |
//		 | r31 r32 r33 |
//	 0x = atan2( r32, r33)
//	 
//	 0y = atan2(-r31, sqrt( ( r32 * r32 ) + ( r33 * r33 )))
//	 0z = atan2( r21, r11)
//
void Matrix3x3ToEulerAngles(vec3 &eulerangles, mat3x3 &mat);

// ----------------------------------------------------------------------------------------------------------------------------

class mat4x4
{
public:
	union {
		struct { float M[16]; };
		struct { float m[4][4]; };
	};
	
	mat4x4();
	~mat4x4();
	mat4x4(const mat4x4 &Matrix);
	mat4x4(const vec4 &col1, const vec4 &col2, const vec4 &col3, const vec4 &col4);
	mat4x4(float c1r1, float c1r2, float c1r3, float c1r4, float c2r1, float c2r2, float c2r3, float c2r4, float c3r1, float c3r2, float c3r3, float c3r4, float c4r1, float c4r2, float c4r3, float c4r4);
	explicit mat4x4(const mat2x2 &Matrix);
	explicit mat4x4(const mat3x3 &Matrix);
	mat4x4& operator = (const mat4x4 &Matrix);
	float& operator [] (int i);
	float* operator & ();
	friend mat4x4 operator * (const mat4x4 &Matrix1, const mat4x4 &Matrix2);
	friend vec4 operator * (const mat4x4 &Matrix, const vec4 &u);
};

#define Mat2x2_zero      mat2x2(0, 0,  0, 0);
#define Mat2x2_identity  mat2x2(1, 0,  0, 1);
#define Mat3x3_zero      mat3x3( 0, 0, 0,  0, 0, 0,  0, 0, 0);
#define Mat3x3_identity  mat3x3(1, 0, 0,  0, 1, 0,  0, 0, 1);
#define Mat4x4_zero     mat4x4(0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0);
#define Mat4x4_identity mat4x4(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1);

mat4x4 inverse(const mat4x4 &Matrix);
mat4x4 look(const vec3 &eye, const vec3 &center, const vec3 &up);
mat4x4 ortho(float left, float right, float bottom, float top, float n, float f);
mat4x4 perspective(float fovy, float aspect, float n, float f);
mat4x4 rotate(float angle, const vec3 &u);
mat4x4 scale(float x, float y, float z);
mat4x4 translate(float x, float y, float z);
mat4x4 transpose(const mat4x4 &Matrix);

// ----------------------------------------------------------------------------------------------------------------------------
// Graphics
// ----------------------------------------------------------------------------------------------------------------------------
//void ComputeNormalWithTriangle(vec3 &destnormal, vec3 &a, vec3 &b, vec3 &c);
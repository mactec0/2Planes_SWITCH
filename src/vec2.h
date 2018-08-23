#pragma once 
 
#include <cmath>

class vec2_t
{
public:
	float x, y;
	inline void Init(float ix, float iy)
	{
		x = ix; y = iy;
	}
	vec2_t() { x = 0.f; y = 0.f; };
	vec2_t(float X, float Y) { x = X; y = Y; };
	vec2_t(float *vec) { x = vec[0]; y = vec[1]; };
	float operator[](int i) const { if (i == 0) return x; return y; };
	float& operator[](int i) { if (i == 0) return x; return y; };

	bool operator==(const vec2_t& v) { if(x==v.x&&y==v.y)return true; return false; }
	bool operator!=(const vec2_t& v) { if(x!=v.x||y!=v.y)return true; return false; }

	inline vec2_t operator-(const vec2_t& v) { return vec2_t(x - v.x, y - v.y); }
	inline vec2_t operator+(const vec2_t& v) { return vec2_t(x + v.x, y + v.y); }
	inline vec2_t operator*(const float n) { return vec2_t(x*n, y*n); }
	inline vec2_t operator-() { return vec2_t(-x, -y); }
	float LengthSqr(void) { return (x*x + y*y); }
	float Length(void) { return (x*x + y*y); }
	float Length2D(void) { return sqrtf(x*x + y*y); }
	float VectorLength(void) { return sqrt(x*x + y*y); }
	inline vec2_t& operator+=(const vec2_t &v) { x += v.x; y += v.y; return *this; }
	inline vec2_t& operator-=(const vec2_t &v) { x -= v.x; y -= v.y; return *this; }
	inline vec2_t& operator/=(const vec2_t &v) { x /= v.x; y /= v.y; return *this; }
	inline vec2_t& operator/=(const float v) { x /= v; y /= v; return *this; }
	float Distance(vec2_t &v) {
		float dx=x-v.x;
		float dy=y-v.y;
		return sqrtf(dx*dx+dy*dy);
	}

}; 

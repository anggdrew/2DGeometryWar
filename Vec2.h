#pragma once
#include"Common.h"

class Vec2 {
public:

	float x = 0;
	float y = 0;
	
	Vec2() = default;
	Vec2(float x_in,float y_in);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	float dist_sqr(const Vec2& rhs) const;
	float length() const;
	void normalize();

};
#pragma once

#include <cmath>

template<typename T>
struct Vector3 {
	T x;
	T y;
	T z;
	Vector3() : x(T()), y(T()), z(T()) { }
	Vector3(T a, T b, T c) : x(a), y(b), z(c) { }

	Vector3<T> operator+(const Vector3<T> &p) {
		Vector3<T> newp;
		newp.x = x + p.x;
		newp.y = y + p.y;
		newp.z = z + p.z;
		return newp;
	}

	Vector3<T>& operator+=(const Vector3<T> &p) {
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	Vector3<T> operator-(const Vector3<T> &p) {
		Vector3<T> newp;
		newp.x = x - p.x;
		newp.y = y - p.y;
		newp.z = z - p.z;
		return newp;
	}

	Vector3<T>& operator-=(const Vector3<T> &p) {
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	Vector3<T> operator*(const T &t) {
		Vector3<T> newp;
		newp.x = x * t;
		newp.y = y * t;
		newp.z = z * t;
		return newp;
	}

	Vector3<T>& operator*=(const T &t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	Vector3<T> operator/(const T &t) {
		Vector3<T> newp;
		newp.x = x / t;
		newp.y = y / t;
		newp.z = z / t;
		return newp;
	}

	Vector3<T>& operator/=(const T &t) {
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}

	T norm() {
		return sqrt(x*x + y*y + z*z);
	}
};

typedef Vector3<float> Point;
#ifndef COLOR_EXT
#define COLOR_EXT

// Colors. Colors (RGBA) aRe stored in Vector4s or in uint32s. The highest bits are red, the lowest ones are alpha.

// #include <iostream>
#include <cstdint>

#include "../utils/math/Vectors.h"

uint32_t rgba(Vector4 v) {
	const float T = 255;
	uint32_t r = (uint32_t)(v.x * T);
	uint32_t g = (uint32_t)(v.y * T);
	uint32_t b = (uint32_t)(v.z * T);
	uint32_t a = (uint32_t)(v.w * T);

	return a + (b<<8) + (g<<16) + (r<<24);
}

Vector4 rgba(uint32_t s) {
	uint32_t r = (s>>24);
	uint32_t g = (s>>16) % (1<<8);
	uint32_t b = (s>>8) % (1<<8);
	uint32_t a = s % (1<<8);

	return Vector4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

Vector3 rgb(uint32_t s) {
	return vec3(rgba(s));
}

uint32_t rgb(Vector3 s) {
	return rgba(fromPoint(s));
}

uint32_t rgb(int r, int g, int b) {
	return (r<<24) + (g<<16) + (b<<8) + 255;
}

uint32_t colmul(uint32_t s, float f) {
	Vector4 x = rgba(s) * BASE::clamp(f, 0, 1);
	// std::cout << rgba(s).to_string() << " " << f << " " << x.to_string() << "\n";
	return rgba(x);
}

Vector4 FullHue(float prop) {
	prop = fmod(prop, 1.0);
	while (prop < 0) prop += 1;
	prop = fmod(prop, 1.0);

	float SS = 1.0 / 3.0;

	float index = fmod(prop, SS) * 3.0;
	if (prop < 1 * SS) return Vector4(1 - index, index, 0, 1);
	if (prop < 2 * SS) return Vector4(0, 1 - index, index, 1);
	if (prop < 3 * SS) return Vector4(index, 0, 1 - index, 1);

	return Vector4(1, 1, 1, 1);
}

#endif
#ifndef COLOR_EXT
#define COLOR_EXT

// Colors. Colors (RGBA) aRe stored in Vector4s or in uint32s. The highest bits are red, the lowest ones are alpha.

#include <cstdint>

#include "../utils/math/Vectors.h"

static uint32_t rgba(Vector4 v) {
	const float T = 255;
	uint32_t r = (uint32_t)(v.x * T);
	uint32_t g = (uint32_t)(v.y * T);
	uint32_t b = (uint32_t)(v.z * T);
	uint32_t a = (uint32_t)(v.w * T);

	return a + (b<<8) + (g<<16) + (r<<24);
}

static Vector4 rgba(uint32_t s) {
	uint32_t r = (s>>24);
	uint32_t g = (s>>16) % (1<<8);
	uint32_t b = (s>>8) % (1<<8);
	uint32_t a = s % (1<<8);

	return Vector4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

static Vector3 rgb(uint32_t s) {
	return vec3(rgba(s));
}

static uint32_t rgb(Vector3 s) {
	return rgba(fromPoint(s));
}

static uint32_t rgb(int r, int g, int b) {
	return (r<<24) + (g<<16) + (b<<8) + 255;
}

static uint32_t colmul(uint32_t s, float f) {
	Vector4 x = rgba(s) * BASE::clamp(f, 0, 1);
	// std::cout << rgba(s).to_string() << " " << f << " " << x.to_string() << "\n";
	return rgba(x);
}

static Vector4 FullHue(float prop) {
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

static Vector3 clampColor(Vector3 s) {
	return Vector3(BASE::clamp(s.x, 0, 1), BASE::clamp(s.y, 0, 1), BASE::clamp(s.z, 0, 1));
}

// RGB and HSV conversions. Hue rotation is measured in revolutions so that 180 degrees is the value 0.5, and 0 is pure red.
static Vector3 rgb2hsv(Vector3 rgb) {
	float r = rgb.x;
	float g = rgb.y;
	float b = rgb.z;

	float xmax = fmax(r, fmax(g, b));
	float xmin = fmin(r, fmin(g, b));

	float value = xmax;
	float chroma = xmax - xmin;

	float saturation = 0;
	if (!BASE::fzero(value)) saturation = chroma / value;

	float hue = 0;
	if (!BASE::fzero(chroma)) {
		if (BASE::fequal(chroma, r)) hue = BASE::frem((g - b) / chroma, 6.0) / 6.0;
		else if (BASE::fequal(chroma, g)) hue = BASE::frem(2 + (b - r) / chroma, 6.0) / 6.0;
		else if (BASE::fequal(chroma, b)) hue = BASE::frem(4 + (r - g) / chroma, 6.0) / 6.0;
	}

	return Vector3(hue, saturation, value);
}

static Vector3 hsv2rgb(Vector3 hsv) {
	float hue = hsv.x;
	float saturation = hsv.y;
	float value = hsv.z;

	float chroma = saturation * value;
	float hprime = hue * 6;

	float bling = hprime;
	while (bling > 2) bling -= 2;

	float x = chroma * (1 - fabs(bling - 1));

	Vector3 res(0, 0, 0);

	switch (BASE::ifloor(hprime)) {
		case 0:
			res = Vector3(chroma, x, 0);
			break;
		case 1:
			res = Vector3(x, chroma, 0);
			break;
		case 2:
			res = Vector3(0, chroma, x);
			break;
		case 3:
			res = Vector3(0, x, chroma);
			break;
		case 4:
			res = Vector3(x, 0, chroma);
			break;
		case 5:
			res = Vector3(chroma, 0, x);
			break;
	}

	return res + (Vector3(1, 1, 1) * (value - chroma));
}

#endif
#ifndef STRINGUTILS_EXT
#define STRINGUTILS_EXT

#include <vector>
#include <string>
#include <cwctype>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "Math.h"

inline bool iswhitespace(std::string s) {
	if (s.length() <= 0) return true;
	for (int i = 0; i < s.length(); i++) {
		if (!iswspace(s[i])) return false;
	}
	return true;
}

inline std::vector<std::string> split(std::string s) {
	std::vector<std::string> res;
	int p = 0;
	int i = 0;
	for (i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			std::string ss = s.substr(p, i - p);
			if (!iswhitespace(ss)) res.push_back(ss);
			p = i + 1;
		}
	}
	std::string ss = s.substr(p, i - p);
	if (!iswhitespace(ss)) res.push_back(ss);
	return res;
}

inline int find(std::string s, char t, int x) {
	for (int i = x; i < s.length(); i++) {
		if (s[i] == t) return i;
	}
	return -1;
}

inline std::string substr(std::string s, int a, int b) {
	return s.substr(a, b - a);
}

inline std::string gradientBlock(float f) {
	std::vector<std::string> res({" ", "░", "▒", "▓", "█"});

	float threshold = 1.0 / (res.size() - 1);

	for (int i = 0; i < res.size() - 1; i++) {
		if (f < threshold * i) return res[i];
	}
	return res[res.size() - 1];
}

inline std::string disp(std::vector<Vector2> v) {
	std::string res = "[";
	for (int i = 0; i < v.size(); i++) {
		if (i) res += ", ";
		res += v[i].to_string();
	}
	return res + "]";
}

template <typename T>
inline std::string disp(std::vector<T> v) {
	std::string res = "[";
	for (int i = 0; i < v.size(); i++) {
		if (i) res += ", ";
		res += std::to_string(v[i]);
	}
	return res + "]";
}

#endif
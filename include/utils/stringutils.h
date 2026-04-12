#ifndef STRINGUTILS_EXT
#define STRINGUTILS_EXT

#include <vector>
#include <string>
#include <cwctype>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "Math.h"

// whitespace
inline bool iswhitespace(std::string s) {
	if (s.length() <= 0) return true;
	for (int i = 0; i < s.length(); i++) {
		if (!iswspace(s[i])) return false;
	}
	return true;
}

// split a string by whitespace
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

// Find the first instance of t in s, starting from index x.
inline int find(std::string s, char t, int x) {
	for (int i = x; i < s.length(); i++) {
		if (s[i] == t) return i;
	}
	return -1;
}

// s[a, b)
inline std::string substr(std::string s, int a, int b) {
	return s.substr(a, b - a);
}

// Converta float to a gradient block
inline std::string gradientBlock(float f) {
	std::vector<std::string> res({" ", "░", "▒", "▓", "█"});

	float threshold = 1.0 / (res.size() - 1);

	for (int i = 0; i < res.size() - 1; i++) {
		if (f < threshold * i) return res[i];
	}
	return res[res.size() - 1];
}

// Print a vector of whatevers
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

// Numbers to strings

inline std::string print_binary(uint64_t x) {
    std::string res = "";
    for (int i = 0; i < 64; i++) {
        res.push_back('0' + (x & 1));
        x >>= 1;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

inline std::string print_hex(uint64_t x) {
    std::string res = "";
    for (int i = 0; i < 16; i++) {
        int val = x & 15;
        if (val < 10) res.push_back('0' + val);
        else res.push_back('A' + (val - 10));
        x >>= 4;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

inline std::string print_binary(uint32_t x) {
    std::string res = "";
    for (int i = 0; i < 32; i++) {
        res.push_back('0' + (x & 1));
        x >>= 1;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

inline std::string print_binary(uint8_t x) {
    std::string res = "";
    for (int i = 0; i < 8; i++) {
        res.push_back('0' + (x & 1));
        x >>= 1;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

inline std::string print_binary(char x) {
    return print_binary((uint8_t)(x));
}


inline std::string print_hex(uint32_t x) {
    std::string res = "";
    for (int i = 0; i < 8; i++) {
        int val = x & 15;
        if (val < 10) res.push_back('0' + val);
        else res.push_back('a' + (val - 10));
        x >>= 4;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

inline std::string print_string_as_binary(std::string s) {
	std::string res = "";
	for (auto c : s) {
		res += print_binary(c) + "\n";
	}
	return res;
}

#endif
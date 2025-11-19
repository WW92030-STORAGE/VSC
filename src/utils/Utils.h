#ifndef XXXXUTILS_EXT
#define XXXXUTILS_EXT

#include <vector>
#include <set>
#include <map>

template <typename T>
inline T* fromvec(std::vector<T> s) {
	int N = s.size();
	T* res = new T[N];
	for (int i = 0; i < N; i++) res[i] = s[i];
	return res;
}


// Push to vector or get the existing index (push or get = pog)
template <typename T>
inline int pog(std::vector<T>& arr, std::map<T, int>& inv, T object) {
	if (inv.find(object) == inv.end()) {
		inv.insert({object, arr.size()});
		arr.push_back(object);
	}
	return inv.at(object);
};

// Addition and scalar multiplication of std::vector

std::vector<float> operator+(std::vector<float> const &a, std::vector<float> const & b) {
    std::vector<float> res;
    for (int i = 0; i < a.size() && i < b.size(); i++) res.push_back(a[i] + b[i]);
    return res;
}

std::vector<float> operator*(std::vector<float> const &a, float b) {
    std::vector<float> res;
    for (int i = 0; i < a.size(); i++) res.push_back(a[i] * b);
    return res;
}

// Make a one hot encoded vector
std::vector<float> ket(int n, int k) {
	std::vector<float> res(n, 0);
	if (k >= 0 && k < n) res[k] = 1;
	return res;
}

#endif
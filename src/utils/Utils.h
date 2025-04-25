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

#endif
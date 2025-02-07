#ifndef XXXXUTILS_EXT
#define XXXXUTILS_EXT

template <typename T>
inline T* fromvec(std::vector<T> s) {
	int N = s.size();
	T* res = new T[N];
	for (int i = 0; i < N; i++) res[i] = s[i];
	return res;
}

#endif
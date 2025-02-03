#ifndef LL_EXT
#define LL_EXT

template <typename T>
struct LLNode {
	T val;
	LLNode<T>* next = nullptr;
	LLNode<T>* prev = nullptr;

	LLNode(T v) {
		val = v;
	}
};

#endif
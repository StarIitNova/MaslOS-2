#pragma once

#include <libm/heap/heap.h>

namespace std {

template<typename T>
class vector {
public:
    vector() = default;
    vector(T *it_f, T *it_b) {
        size_t c = it_b - it_f;
        if (c == 0) return;

        reserve(c);
        for (size_t i = 0; i < c; ++i)
            mData[i] = it_f[i];

        mSize = c;
    }

    ~vector() {
        if (mData) _Free(mData);
    }

    void push_back(const T& value) {
        if (mSize >= mCapacity)
            reserve(mCapacity ? mCapacity * 2 : 4);

        mData[mSize++] = value;
    }

    void reserve(size_t cap) {
        if (cap <= mCapacity) return;
        T* newData = (T*)Malloc(cap * sizeof(T));
        for (size_t i = 0; i < mSize; ++i)
            newData[i] = mData[i];

        if (mData) _Free(mData);
        mData = newData;
        mCapacity = cap;
    }

    T& operator[](size_t i) { return mData[i]; }
	const T& operator[](size_t i) const { return mData[i]; }

	size_t size() const { return mSize; }

	bool empty() const { return mSize == 0; }

private:
    T* mData = nullptr;
    size_t mSize = 0;
    size_t mCapacity = 0;
};

}

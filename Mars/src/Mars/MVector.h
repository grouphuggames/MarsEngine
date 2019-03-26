#pragma once
#include "Core.h"


template <typename T>
class MVector
{
	T* data;
	s32 capacity;
	s32 size;

	void IncreaseCapacity(s32 sz)
	{
		if (sz <= capacity) return;

		T* new_arr = new T[sz];

		for (s32 i = 0; i < capacity; ++i) { new_arr[i] = data[i]; }
		capacity = sz;

		delete[] data;
		data = new_arr;
	}

public:
	MVector<T>() : capacity(10), size(0) { data = new T[capacity]; }
	MVector<T>(s32 _size) : capacity(_size + 1), size(0) { data = new T[capacity]; }

	s32 Size() const { return size; }

	bool Find(T t)
	{
		for (s32 i = 0; i < size; ++i)
		{
			if (t == data[i]) return true;
		}

		return false;
	}

	T &MVector<T>::operator[](s32 index) { return data[index]; }

	void PushBack(T val)
	{
		if (size >= capacity) IncreaseCapacity(2 * capacity);

		data[size] = val;
		++size;
	}

	~MVector() { delete[] data; }

};

#pragma once
#include "Core.h"


namespace Mars
{
	template <typename T>
	class MVector
	{
		T* data;
		s32 capacity;
		s32 size;

		void Resize(s32 sz)
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

		T &MVector<T>::operator[](s32 index) { return data[index]; }

		s32 Size() const { return size; }

		T* begin() { return &data[0]; }
		T* end() { return &data[size]; }

		bool Find(T t)
		{
			for (s32 i = 0; i < size; ++i)
			{
				if (t == data[i]) return true;
			}

			return false;
		}

		s32 FindIndex(T t)
		{
			for (s32 i = 0; i < size; ++i)
			{
				if (t == data[i]) return i;
			}

			return -1;
		}

		void PushBack(T val)
		{
			if (size >= capacity) Resize(2 * capacity);

			data[size] = val;
			++size;
		}

		void Remove(s32 index)
		{
			*this[index] = NULL;
		}

		~MVector() { delete[] data; }

	};
}

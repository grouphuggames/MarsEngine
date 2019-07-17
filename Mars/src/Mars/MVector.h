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
			//if (sz <= capacity) return;		-- idk why this line is here and i'm not sure if it's important...if something breaks try to comment this out

			T* new_arr = new T[sz];

			for (s32 i = 0; i < sz; ++i) { new_arr[i] = data[i]; }
			capacity = sz;
			// size = sz;

			delete[] data;
			data = new_arr;
		}

	public:
		MVector<T>() : capacity(10), size(0) { data = new T[capacity]; }
		MVector<T>(s32 _size) : capacity(_size + 1), size(0) { data = new T[capacity]; }

		MVector<T>(const MVector<T>& other) : size(other.size), capacity(other.capacity)		// vectors should be passed into functions by reference for modifying functions and const reference for non-modifying functions
		{																						// this is for performance reasons, don't want to copy vectors around unneccessarily
			data = new T[capacity];
			for (s32 i = 0; i < size; ++i)
			{
				data[i] = other.data[i];
			}
		}

		s32 Size() const { return size; }

		T* begin() { return &data[0]; }
		T* end() { return &data[size]; }

		T& operator[](s32 index) { return data[index]; }

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
        
        //void PushBack(T&& val);

		void Remove(s32 index)
		{
			data[index] = data[size - 1];
			Resize(size - 1);
		}

		~MVector()
		{
			delete[] data;
			data = nullptr;
		}
	};
}

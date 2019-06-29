#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif
#include <fstream>
#include <string>
#include "MVector.h"
#include "vec3.h"
#include "Core.h"
#include "Utils.h"


namespace Mars
{
#ifdef _WIN32
	void CreateWin32DebugConsole()
	{
		LPCTSTR console_name = "Debug Console";
		AllocConsole();
		SetConsoleTitle(console_name);
		freopen("CONOUT$", "w", stdout);
	}
#endif

	struct HotInt
	{
		HotInt() = default;
		HotInt(u32 _key, s32* _value) : key(_key), value(_value) {}

		u32 key;
		s32* value;
	};

	struct HotFloat
	{
		HotFloat() = default;
		HotFloat(u32 _key, f32* _value) : key(_key), value(_value) {}

		u32 key;
		f32* value;
	};

	struct HotBool
	{
		HotBool() = default;
		HotBool(u32 _key, bool* _value) : key(_key), value(_value) {}

		u32 key;
		bool* value;
	};

	struct HotVec3
	{
		HotVec3() = default;
		HotVec3(u32 _key, vec3* _value) : key(_key), value(_value) {}

		u32 key;
		vec3* value;
	};

	struct HRVars
	{
		MVector<HotInt> ints;
		MVector<HotFloat> floats;
		MVector<HotBool> bools;
		MVector<HotVec3> vec3s;
		// look into making it possible for programmer to add custom data types from the game into this list...
	};

	MARS_API HRVars hot_reload_vars;

	static void AddHot(u32 name_hash, s32* var_ptr) { hot_reload_vars.ints.PushBack(HotInt(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, f32* var_ptr) { hot_reload_vars.floats.PushBack(HotFloat(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, bool* var_ptr) { hot_reload_vars.bools.PushBack(HotBool(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, vec3* var_ptr) { hot_reload_vars.vec3s.PushBack(HotVec3(name_hash, var_ptr)); }

	u32 string_hash(const char* data, size_t data_length, const char* salt, u32 salt_length)
	{
		u32 hash = 0;
		unsigned char* hash_ptr = reinterpret_cast<unsigned char*>(&hash);

		for (u32 i = 0; i < salt_length; i++)
			hash_ptr[i % 4] = hash_ptr[i % 4] ^ salt[i];

		for (u32 j = 0; j < data_length; j++)
			hash_ptr[j % 4] = hash_ptr[j % 4] ^ data[j];

		return hash;
	}

#define ADDHOT(VAR)	AddHot(string_hash(#VAR, strlen(#VAR), "MARS", 5), &(VAR))

	void HotReload()
	{
		std::ifstream file("..\\..\\Mars\\res\\HotReload.txt");

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				size_t space = line.find(' ');
				u32 name_hash = string_hash(line.substr(0, space).c_str(), (u32)space, "MARS", 5);
				std::string value = line.substr(space + 1);
		
				for (auto a : hot_reload_vars.floats)
				{
					if (a.key == name_hash)
						*a.value = std::stof(value);
				}
				for (auto b : hot_reload_vars.ints)
				{
					if (b.key == name_hash)
						*b.value = std::stoi(value);
				}
				for (auto c : hot_reload_vars.bools)
				{
					if (c.key == name_hash)
						*c.value = static_cast<bool>(std::stoi(value));
				}
				for (auto d : hot_reload_vars.vec3s)
				{
					if (d.key == name_hash)
					{
						f32 vals[] = { 0.f, 0.f, 0.f };
						for (u8 i = 0; i < 3; i++)
						{
							size_t new_space = value.find(' ');
							std::string new_value = value.substr(0, new_space);
							vals[i] = std::stof(new_value);
		
							if (i < 2)
								value = value.substr(new_space + 1);
						}
		
						*d.value = vec3(vals[0], vals[1], vals[2]);
					}
				}
			}
			MARS_CORE_INFO("Hot reload success");
		}
		else
		{
			MARS_CORE_ERROR("Error could not find file");
		}
	}
}

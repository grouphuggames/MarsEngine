#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include "MVector.h"
#include "vec3.h"
#include "Core.h"


namespace Mars
{
	void CreateWin32DebugConsole()
	{
		LPCTSTR console_name = "Debug Console";
		AllocConsole();
		SetConsoleTitle(console_name);
		freopen("CONOUT$", "w", stdout);
	}

#define SALT "MARS"
#define SIZE_OF_SALT 5

	struct HRVars
	{
		MVector<std::tuple<u32, s32*>> ints;
		MVector<std::tuple<u32, f32*>> floats;
		MVector<std::tuple<u32, bool*>> bools;
		MVector<std::tuple<u32, vec3*>> vec3s;
	};

	MARS_API HRVars hot_reload_vars;

	static void AddHot(u32 name_hash, s32* var_ptr) { hot_reload_vars.ints.PushBack(std::make_tuple(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, f32* var_ptr) { hot_reload_vars.floats.PushBack(std::make_tuple(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, bool* var_ptr) { hot_reload_vars.bools.PushBack(std::make_tuple(name_hash, var_ptr)); }
	static void AddHot(u32 name_hash, vec3* var_ptr) { hot_reload_vars.vec3s.PushBack(std::make_tuple(name_hash, var_ptr)); }

	u32 string_hash(const char* data, u32 data_length, const char* salt, u32 salt_length)
	{
		u32 hash = 0;
		unsigned char* hash_ptr = reinterpret_cast<unsigned char*>(&hash);

		for (u32 i = 0; i < salt_length; i++)
			hash_ptr[i % 4] = hash_ptr[i % 4] ^ salt[i];

		for (u32 j = 0; j < data_length; j++)
			hash_ptr[j % 4] = hash_ptr[j % 4] ^ data[j];

		return hash;
	}

#define ADDHOT(VAR)	AddHot(string_hash(#VAR, strlen(#VAR), SALT, SIZE_OF_SALT), &(VAR))

	// for some reason, vectors (MVector AND std::vector) are getting emptied before they can be processed when this function is called...
	// for right now all hot reloading is broken...
	// dllimport/dllexport might be the key to fixing it... similar things were happening to hwnd from Core.h and now that stuff is fixed
	void HotReload()
	{
		std::ifstream file("..\\..\\Mars\\res\\HotReload.txt");

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				size_t space = line.find(' ');
				u32 name_hash = string_hash(line.substr(0, space).c_str(), (u32)space, SALT, SIZE_OF_SALT);
				std::string value = line.substr(space + 1);
		
				for (auto a : hot_reload_vars.floats)
				{
					if (std::get<0>(a) == name_hash)
						*std::get<1>(a) = std::stof(value);
				}
				for (auto b : hot_reload_vars.ints)
				{
					if (std::get<0>(b) == name_hash)
						*std::get<1>(b) = std::stoi(value);
				}
				for (auto c : hot_reload_vars.bools)
				{
					if (std::get<0>(c) == name_hash)
						*std::get<1>(c) = static_cast<bool>(std::stoi(value));
				}
				for (auto d : hot_reload_vars.vec3s)
				{
					if (std::get<0>(d) == name_hash)
					{
						float vals[] = { 0.f, 0.f, 0.f };
						for (u8 i = 0; i < 3; i++)
						{
							size_t new_space = value.find(' ');
							std::string new_value = value.substr(0, new_space);
							vals[i] = std::stof(new_value);
		
							if (i < 2)
								value = value.substr(new_space + 1);
						}
		
						*std::get<1>(d) = vec3(vals[0], vals[1], vals[2]);
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

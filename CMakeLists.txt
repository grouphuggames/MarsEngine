cmake_minimum_required(VERSION 3.13)
project(MarsEngine VERSION 0.1.0)

if (MSVC)
	add_compile_options("/std:c++17")
	
	if (WIN32)
		add_link_options("/SUBSYSTEM:WINDOWS")
	endif()
endif()

add_subdirectory(Mars)
add_subdirectory(TestGame)
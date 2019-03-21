cmake_minimum_required(VERSION 3.13)
project(MarsEngine VERSION 0.1.0)

if (MSVC)
	add_compile_options("/std:c++17")
	add_compile_options("/Oi")
	add_compile_options("/Ob1")
	add_compile_options("/Gv")
	add_compile_options("/fp:fast")
	add_link_options("/SUBSYSTEM:WINDOWS")
endif()

add_subdirectory(Mars)
add_subdirectory(TestGame)
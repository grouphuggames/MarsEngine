file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(Mars SHARED ${headers} ${sources})

target_include_directories(Mars PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (WIN32)
	target_compile_definitions(Mars PUBLIC ME_PLATFORM_WINDOWS)
endif()

target_compile_definitions(Mars PRIVATE ME_BUILD_DLL)
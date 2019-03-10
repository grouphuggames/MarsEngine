file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(TestGame ${sources})
target_link_libraries(TestGame Mars)
target_include_directories(TestGame PUBLIC "${CMAKE_SOURCE_DIR}/Mars/src")
add_custom_command(TARGET TestGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Mars> $<TARGET_FILE_DIR:TestGame>)
add_executable(TestGame src/Game.cpp)
target_link_libraries(TestGame Mars)
add_custom_command(TARGET TestGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Mars> $<TARGET_FILE_DIR:TestGame>)
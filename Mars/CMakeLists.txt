add_library(Mars SHARED src/Test.h src/Test.cpp)

target_include_directories(Mars PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
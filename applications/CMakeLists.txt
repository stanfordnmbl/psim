include_directories(${PROJECT_SOURCE_DIR})
add_executable(psim-app psim.cpp)
set_target_properties(psim-app PROPERTIES OUTPUT_NAME psim)
include_directories("${PROJECT_SOURCE_DIR}" "${OpenSim_INCLUDE_DIRS}")
link_directories(${OpenSim_LIB_DIR})
add_executable(psim-app psim.cpp)
target_link_libraries(psim-app psim)
set_target_properties(psim-app PROPERTIES OUTPUT_NAME psim)

install(TARGETS psim-app DESTINATION bin)

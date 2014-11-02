file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

include_directories(${OpenSim_INCLUDE_DIRS})
link_directories(${OpenSim_LIB_DIR})

add_library(psim SHARED ${SOURCES} ${HEADERS})

set_target_properties(psim PROPERTIES DEFINE_SYMBOL OSIMPSIM_EXPORTS)

target_link_libraries(psim ${OpenSim_LIBRARIES})


CONFIGURE_FILE(PSimConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/PSimConfig.cmake" @ONLY)

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/PSimConfig.cmake"
    DESTINATION cmake)

install(EXPORT PSimTargets DESTINATION cmake)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file(Doxyfile.in "${doxyfile}" @ONLY)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} "${doxyfile}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION doc)
endif()

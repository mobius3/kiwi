include(CMakeParseArguments)
macro(kiwires)
    set(ONEVALUEARGS HEADER BODY)
    set(MULTIVALUEARGS RESOURCES)
    cmake_parse_arguments(KIWIRES "" "${ONEVALUEARGS}" "${MULTIVALUEARGS}" ${ARGN})
    try_compile(FILE2C_RESULT ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/file2c.c COPY_FILE "${CMAKE_BINARY_DIR}/file2c${CMAKE_EXECUTABLE_SUFFIX}")
    execute_process(
            WORKING_DIRECTORY ${KIWI_ROOT}
            COMMAND
                ${CMAKE_BINARY_DIR}/file2c${CMAKE_EXECUTABLE_SUFFIX}
                    ${CMAKE_CURRENT_BINARY_DIR}/${KIWIRES_HEADER}
                    ${CMAKE_CURRENT_BINARY_DIR}/${KIWIRES_BODY}
                    ${KIWIRES_RESOURCES}
           )
endmacro()

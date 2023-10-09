# Get the remill include directories from the target
if(NOT TARGET remill_settings)
    message(FATAL_ERROR "Could not find target 'remill_settings' (did remill update?)")
endif()
get_target_property(HELPER_REMILL_INCLUDE_DIR remill_settings INTERFACE_INCLUDE_DIRECTORIES)
if(HELPER_REMILL_INCLUDE STREQUAL "HELPER_REMILL_INCLUDE_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not determine remill include directory")
endif()

# Locate the clang executable
set(HELPER_CLANG_EXECUTABLE ${LLVM_TOOLS_BINARY_DIR}/clang++${CMAKE_EXECUTABLE_SUFFIX})
if(NOT EXISTS "${HELPER_CLANG_EXECUTABLE}")
    message(FATAL_ERROR "Could not find: ${HELPER_CLANG_EXECUTABLE}")
endif()

set(HELPER_CLANG_FLAGS
    -emit-llvm
    -std=c++17
    -O3
    -Werror
    -Wno-gnu-inline-cpp-without-extern
    -Wno-return-type-c-linkage

    -fno-discard-value-names
    -fstrict-aliasing
    -fno-slp-vectorize
    -mllvm -enable-tbaa=true

    "-I${HELPER_REMILL_INCLUDE_DIR}"
)

# Add the MacOS SDK sysroot and the LLVM C++ includes folder
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND HELPER_CLANG_FLAGS "-isysroot${CMAKE_OSX_SYSROOT}")
    list(APPEND HELPER_CLANG_FLAGS "-I${LLVM_INCLUDE_DIR}/c++/v1")
endif()

message(STATUS "[helpers] Remill include: ${HELPER_REMILL_INCLUDE_DIR}")
message(STATUS "[helpers] Clang: ${HELPER_CLANG_EXECUTABLE}")
message(STATUS "[helpers] Flags: ${HELPER_CLANG_FLAGS}")

add_custom_target(helpers)

function(add_helper arch)
    # Generate a CMake script to compile the helpers
    # This allows the user to do project-specific helpers and easily recompile them
    message(STATUS "[helpers] Adding architecture: ${arch}")
    set(HELPER_BINARY_DIR "${CMAKE_BINARY_DIR}/helpers/${arch}")
    set(HELPER_SCRIPT "${HELPER_BINARY_DIR}/build.cmake")
    configure_file("build.cmake.in" "${HELPER_SCRIPT}" @ONLY)

    # Determine the inputs/outputs for the custom command
    set(HELPER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${arch}")
    set(HELPER_SOURCES
        "${HELPER_DIR}/RemillHelpers.cpp"
        "${HELPER_DIR}/RemillHotpatch.cpp"
    )
    set(HELPER_OUTPUTS "")
    foreach(source ${HELPER_SOURCES})
        if(EXISTS "${source}")
            get_filename_component(name "${source}" NAME_WE)
            list(APPEND HELPER_OUTPUTS
                "${HELPER_BINARY_DIR}/${name}.ll"
                "${HELPER_BINARY_DIR}/${name}.bc"
            )
        endif()
    endforeach()

    # Create a custom target to build it automatically when any of the sources are updated
    add_custom_command(
        OUTPUT ${HELPER_OUTPUTS}
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${HELPER_DIR}" "${HELPER_BINARY_DIR}"
        COMMAND "${CMAKE_COMMAND}" -P "${HELPER_SCRIPT}"
        DEPENDS ${HELPER_SOURCES} ${HELPER_SCRIPT}
        WORKING_DIRECTORY "${HELPER_BINARY_DIR}"
        COMMENT "Building helpers for ${arch}"
    )
    add_custom_target("helpers-${arch}" ALL
        DEPENDS ${HELPER_OUTPUTS}
        SOURCES ${HELPER_SOURCES}
    )
    add_dependencies(helpers "helpers-${arch}")
endfunction()

add_helper(aarch64)
add_helper(x86_64)

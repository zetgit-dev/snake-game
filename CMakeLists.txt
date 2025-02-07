cmake_minimum_required(VERSION 3.10)

set ( PROJECT_NAME snake )

project ( ${PROJECT_NAME}
    DESCRIPTION "snake game using ncurses"
    VERSION 0.1
    LANGUAGES CXX
)

set ( CMAKE_CXX_STANDARD_REQUIRED ON  )
set ( CMAKE_CXX_EXTENSIONS        OFF )
set ( CMAKE_CXX_STANDARD          14  )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )


# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#     add_compile_options (-fdiagnostics-color=always)

# elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#     add_compile_options (-fcolor-diagnostics)
# endif()


file(
    GLOB RESOURCES
        ${CMAKE_SOURCE_DIR}/include/*.cxx
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CURSES_INCLUDE_DIRS}
)

add_executable ( ${PROJECT_NAME}
    ${RESOURCES}
    ${CMAKE_SOURCE_DIR}/src/main.cxx
)

find_package(Curses REQUIRED)

target_compile_options ( ${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:
            -g3
            -glldb
            -O0
            -fsanitize=address
            -Wall
            -Werror
            -Wextra
            -Wpedantic
            -fno-inline
            -fno-optimize-sibling-calls
            -fdiagnostics-show-option
            -fno-asynchronous-unwind-tables
            -fsanitize=address
        >

        $<$<CONFIG:Release>:
            -Os
            -g0
            # -faddrsig
            -Wl,--icf=sage,-strippe-all
            -fomit-frame-pointer
            -march=native
        >

        # -stdlib=libstdc++

        # -flto
        # -pipe
        -Wno-unused-command-line-argument
)

# target_link_options( ${PROJECT_NAME}
#     PRIVATE
#         $<$<CONFIG:Debug>:-fsanitize=address>
#         -fuse-ld=lld
# )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CURSES_LIBRARIES}
)

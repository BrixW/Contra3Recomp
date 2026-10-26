cmake_minimum_required(VERSION 3.16)
project(Contra3Recomp VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

add_library(contra3_recomp STATIC
    src/snes_runtime.c
    src/snes_ppu.c
    src/snes_apu.c
    src/snes_movie.c
    src/snes_savestate.c
    src/snes_rom.c
    src/snes_mainthread.c
    src/contra3_extras.c
    src/contra3_events.c
    src/c3_config.c
    src/c3_overlay.c
    src/c3_rumble.c
    src/bank_00.c
    src/bank_01.c
    src/bank_02.c
    src/bank_03.c
    src/bank_04.c
    src/bank_1d.c
    src/bank_data_manifest.c
    src/dispatch.c
)

target_include_directories(contra3_recomp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# The preemptible main thread uses fibers on Windows and a strict-handoff pthread elsewhere
if(NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(contra3_recomp PUBLIC Threads::Threads)
endif()

# Frontends: the native Win32 runner on Windows, the SDL2 runner everywhere else
# (and optionally on Windows as contra3_sdl).
if(WIN32)
    option(CONTRA3_SDL "Also build the SDL2 frontend (contra3_sdl.exe)" OFF)
    add_executable(contra3 src/win32_main.c src/win32_gl.c src/gl_present.c)
    target_link_libraries(contra3 PRIVATE contra3_recomp opengl32 gdi32 winmm comdlg32)
    set(CONTRA3_SDL_TARGET contra3_sdl)
else()
    option(CONTRA3_SDL "Build the SDL2 frontend" ON)
    set(CONTRA3_SDL_TARGET contra3)
endif()

if(CONTRA3_SDL)
    find_package(SDL2 CONFIG QUIET)
    if(TARGET SDL2::SDL2)
        set(CONTRA3_SDL_LIBS SDL2::SDL2)
        if(TARGET SDL2::SDL2main)
            list(PREPEND CONTRA3_SDL_LIBS SDL2::SDL2main)
        endif()
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
        set(CONTRA3_SDL_LIBS PkgConfig::SDL2)
    endif()
    add_executable(${CONTRA3_SDL_TARGET} src/sdl_main.c src/gl_present.c)
    target_link_libraries(${CONTRA3_SDL_TARGET} PRIVATE contra3_recomp ${CONTRA3_SDL_LIBS})
endif()

if(TARGET contra3)
    install(TARGETS contra3 RUNTIME DESTINATION bin)
endif()

enable_testing()

# ROM-free tests (config, overlay, rumble, movies, inflate) - run everywhere, including CI
add_executable(contra3_unit_test tests/unit_test.c)
target_link_libraries(contra3_unit_test PRIVATE contra3_recomp)
add_test(NAME contra3_unit_test COMMAND contra3_unit_test ${CMAKE_CURRENT_BINARY_DIR})

# Game tests need the ROM, which is never part of the repository
add_executable(contra3_smoke_test tests/smoke_test.c)
target_link_libraries(contra3_smoke_test PRIVATE contra3_recomp)
add_executable(contra3_replay_test tests/replay_test.c)
target_link_libraries(contra3_replay_test PRIVATE contra3_recomp)

# Tests locate the ROM through $CONTRA3_ROM; default to the project's sibling ROM folder
set(CONTRA3_ROM "${CMAKE_CURRENT_SOURCE_DIR}/../ROM/Contra III - The Alien Wars (USA).sfc"
    CACHE FILEPATH "Contra III (USA) ROM image used by the tests")
if(EXISTS "${CONTRA3_ROM}")
    add_test(NAME contra3_smoke_test COMMAND contra3_smoke_test)
    add_test(NAME contra3_replay_test
             COMMAND contra3_replay_test --golden ${CMAKE_CURRENT_SOURCE_DIR}/tests/golden)
    set_tests_properties(contra3_smoke_test contra3_replay_test PROPERTIES ENVIRONMENT "CONTRA3_ROM=${CONTRA3_ROM}")
else()
    message(STATUS "CONTRA3_ROM not found (${CONTRA3_ROM}): smoke and golden replay tests are not registered")
endif()

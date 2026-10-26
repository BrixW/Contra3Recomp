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

if(WIN32)
    add_executable(contra3 src/win32_main.c src/win32_gl.c src/win32_config.c)
    target_link_libraries(contra3 PRIVATE contra3_recomp opengl32 gdi32 winmm comdlg32)
endif()

enable_testing()
add_executable(contra3_smoke_test tests/smoke_test.c)
target_link_libraries(contra3_smoke_test PRIVATE contra3_recomp)
add_test(NAME contra3_smoke_test COMMAND contra3_smoke_test)

add_executable(contra3_replay_test tests/replay_test.c)
target_link_libraries(contra3_replay_test PRIVATE contra3_recomp)
add_test(NAME contra3_replay_test
         COMMAND contra3_replay_test --golden ${CMAKE_CURRENT_SOURCE_DIR}/tests/golden)

# Tests locate the ROM through $CONTRA3_ROM; default to the project's sibling ROM folder
set(CONTRA3_ROM "${CMAKE_CURRENT_SOURCE_DIR}/../ROM/Contra III - The Alien Wars (USA).sfc"
    CACHE FILEPATH "Contra III (USA) ROM image used by the tests")
set_tests_properties(contra3_smoke_test contra3_replay_test PROPERTIES ENVIRONMENT "CONTRA3_ROM=${CONTRA3_ROM}")

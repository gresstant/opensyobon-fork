# This CMake build configuration is written for Windows + MSVC16. Currently, no cross-platform support is planned.
# You should install libraries listed below, and create a folder named "libs" that includes .DLL files required.

cmake_minimum_required (VERSION 3.0)

project (OSAFork)

# MSVC requires "/utf-8" for UTF-8 files without BOM
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(OSAFork main.cpp loadg.cpp DxLib.cpp draw.cpp draw.h entities/mario.cpp entities/mario.h) #entities/block.cpp entities/block.h)

# I'm listing versions that I uses to get it working.
# But still you can try other versions that are different from mine.
target_link_libraries(OSAFork SDL.lib SDLmain.lib)  # SDL 1.2 (not 2.0)
target_link_libraries(OSAFork SDL_gfx.lib)          # SDL_gfx 2.0 (from ferzkopp.net)
target_link_libraries(OSAFork SDL_image.lib)        # SDL_image 1.2
target_link_libraries(OSAFork SDL_mixer.lib)        # SDL_mixer 1.2
target_link_libraries(OSAFork SDL_ttf.lib)          # SDL_ttf 2.0

add_custom_command(TARGET OSAFork POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/libs" $<TARGET_FILE_DIR:OSAFork>)    # remember to create the "libs" folder!
add_custom_command(TARGET OSAFork POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/BGM" $<TARGET_FILE_DIR:OSAFork>/BGM)
add_custom_command(TARGET OSAFork POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:OSAFork>/res)
add_custom_command(TARGET OSAFork POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/SE" $<TARGET_FILE_DIR:OSAFork>/SE)

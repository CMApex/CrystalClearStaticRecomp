cmake_minimum_required(VERSION 3.16)
project(Crystal_Clear C CXX)

include(CheckIPOSupported)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Migrate stale generated-project cache entries when the default profile changes.
set(GBRECOMP_PROFILE_CACHE_VERSION "2")
set(GBRECOMP_APPLIED_PROFILE_CACHE_VERSION "0" CACHE STRING "Internal generated-project profile version")
if(NOT GBRECOMP_APPLIED_PROFILE_CACHE_VERSION STREQUAL GBRECOMP_PROFILE_CACHE_VERSION)
    set(GBRECOMP_ENABLE_IPO OFF CACHE BOOL "Enable interprocedural optimization/LTO for non-Debug builds" FORCE)
    set(GBRECOMP_ENABLE_STRIP ON CACHE BOOL "Strip symbols from generated executables in non-Debug builds" FORCE)
    set(GBRECOMP_GENERATED_OPT_LEVEL "1" CACHE STRING "Optimization level for the generated ROM source files" FORCE)
    set(GBRECOMP_APPLIED_PROFILE_CACHE_VERSION "${GBRECOMP_PROFILE_CACHE_VERSION}" CACHE STRING "Internal generated-project profile version" FORCE)
endif()

# Default to a smaller, iteration-friendly profile for generated projects.
# Benchmark and release builds can still opt into Release/-O3/IPO explicitly.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Build type" FORCE)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_compile_options(-O2 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        add_compile_options(-Os)
    else()
        add_compile_options(-O3)
    endif()
    add_compile_options(-ffunction-sections -fdata-sections)
endif()

# Runtime library path (relative to this output directory)
set(GBRT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../runtime")

# Find SDL2
find_package(SDL2 REQUIRED)

# Create runtime library with PPU and platform support
add_library(gbrt STATIC
    ${GBRT_DIR}/src/gbrt.c
    ${GBRT_DIR}/src/differential.c
    ${GBRT_DIR}/src/ppu.c
    ${GBRT_DIR}/src/audio.c
    ${GBRT_DIR}/src/audio_stats.c
    ${GBRT_DIR}/src/interpreter.c
    ${GBRT_DIR}/src/platform_sdl.cpp
)

# Vendored Dear ImGui subset
target_sources(gbrt PRIVATE
    ${GBRT_DIR}/vendor/imgui/imgui.cpp
    ${GBRT_DIR}/vendor/imgui/imgui_draw.cpp
    ${GBRT_DIR}/vendor/imgui/imgui_tables.cpp
    ${GBRT_DIR}/vendor/imgui/imgui_widgets.cpp
    ${GBRT_DIR}/vendor/imgui/backends/imgui_impl_sdl2.cpp
    ${GBRT_DIR}/vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(gbrt PUBLIC 
    ${GBRT_DIR}/include
    ${GBRT_DIR}/vendor/imgui
)
target_link_libraries(gbrt PUBLIC SDL2::SDL2)
target_compile_definitions(gbrt PUBLIC GB_HAS_SDL2)

# Main executable
add_executable(Crystal_Clear
    Crystal_Clear_main.c
    Crystal_Clear.c
    Crystal_Clear_dispatch_chunk_0.c
    Crystal_Clear_dispatch_chunk_1.c
    Crystal_Clear_dispatch_chunk_2.c
    Crystal_Clear_dispatch_chunk_3.c
    Crystal_Clear_dispatch_chunk_4.c
    Crystal_Clear_funcs_0.c
    Crystal_Clear_funcs_1.c
    Crystal_Clear_funcs_2.c
    Crystal_Clear_funcs_3.c
    Crystal_Clear_funcs_4.c
    Crystal_Clear_funcs_5.c
    Crystal_Clear_funcs_6.c
    Crystal_Clear_funcs_7.c
    Crystal_Clear_funcs_8.c
    Crystal_Clear_funcs_9.c
    Crystal_Clear_funcs_10.c
    Crystal_Clear_funcs_11.c
    Crystal_Clear_funcs_12.c
    Crystal_Clear_funcs_13.c
    Crystal_Clear_funcs_14.c
    Crystal_Clear_funcs_15.c
    Crystal_Clear_funcs_16.c
    Crystal_Clear_funcs_17.c
    Crystal_Clear_funcs_18.c
    Crystal_Clear_funcs_19.c
    Crystal_Clear_funcs_20.c
    Crystal_Clear_funcs_21.c
    Crystal_Clear_funcs_22.c
    Crystal_Clear_funcs_23.c
    Crystal_Clear_funcs_24.c
    Crystal_Clear_funcs_25.c
    Crystal_Clear_funcs_26.c
    Crystal_Clear_funcs_27.c
    Crystal_Clear_funcs_28.c
    Crystal_Clear_funcs_29.c
    Crystal_Clear_funcs_30.c
    Crystal_Clear_funcs_31.c
    Crystal_Clear_funcs_32.c
    Crystal_Clear_funcs_33.c
    Crystal_Clear_funcs_34.c
    Crystal_Clear_funcs_35.c
    Crystal_Clear_funcs_36.c
    Crystal_Clear_funcs_37.c
    Crystal_Clear_funcs_38.c
    Crystal_Clear_funcs_39.c
    Crystal_Clear_funcs_40.c
    Crystal_Clear_funcs_41.c
    Crystal_Clear_funcs_42.c
    Crystal_Clear_funcs_43.c
    Crystal_Clear_funcs_44.c
    Crystal_Clear_funcs_45.c
    Crystal_Clear_funcs_46.c
    Crystal_Clear_funcs_47.c
    Crystal_Clear_funcs_48.c
    Crystal_Clear_funcs_49.c
    Crystal_Clear_funcs_50.c
    Crystal_Clear_funcs_51.c
    Crystal_Clear_funcs_52.c
    Crystal_Clear_funcs_53.c
    Crystal_Clear_funcs_54.c
    Crystal_Clear_funcs_55.c
    Crystal_Clear_funcs_56.c
    Crystal_Clear_funcs_57.c
    Crystal_Clear_funcs_58.c
    Crystal_Clear_funcs_59.c
    Crystal_Clear_funcs_60.c
    Crystal_Clear_funcs_61.c
    Crystal_Clear_funcs_62.c
    Crystal_Clear_funcs_63.c
    Crystal_Clear_funcs_64.c
    Crystal_Clear_funcs_65.c
    Crystal_Clear_funcs_66.c
    Crystal_Clear_funcs_67.c
    Crystal_Clear_funcs_68.c
    Crystal_Clear_rom.c
)

# Generated ROM code defaults to a smaller manual-testing profile.
# Raise the optimization level or enable IPO/LTO explicitly for benchmark/release builds.
set(GBRECOMP_GENERATED_OPT_LEVEL "1" CACHE STRING "Optimization level for the generated ROM source files")
set_property(CACHE GBRECOMP_GENERATED_OPT_LEVEL PROPERTY STRINGS 0 1 2 3)
option(GBRECOMP_ENABLE_IPO "Enable interprocedural optimization/LTO for non-Debug builds" OFF)
option(GBRECOMP_ENABLE_STRIP "Strip symbols from generated executables in non-Debug builds" ON)
set(GBRECOMP_GENERATED_SOURCES
    Crystal_Clear.c
    Crystal_Clear_dispatch_chunk_0.c
    Crystal_Clear_dispatch_chunk_1.c
    Crystal_Clear_dispatch_chunk_2.c
    Crystal_Clear_dispatch_chunk_3.c
    Crystal_Clear_dispatch_chunk_4.c
    Crystal_Clear_funcs_0.c
    Crystal_Clear_funcs_1.c
    Crystal_Clear_funcs_2.c
    Crystal_Clear_funcs_3.c
    Crystal_Clear_funcs_4.c
    Crystal_Clear_funcs_5.c
    Crystal_Clear_funcs_6.c
    Crystal_Clear_funcs_7.c
    Crystal_Clear_funcs_8.c
    Crystal_Clear_funcs_9.c
    Crystal_Clear_funcs_10.c
    Crystal_Clear_funcs_11.c
    Crystal_Clear_funcs_12.c
    Crystal_Clear_funcs_13.c
    Crystal_Clear_funcs_14.c
    Crystal_Clear_funcs_15.c
    Crystal_Clear_funcs_16.c
    Crystal_Clear_funcs_17.c
    Crystal_Clear_funcs_18.c
    Crystal_Clear_funcs_19.c
    Crystal_Clear_funcs_20.c
    Crystal_Clear_funcs_21.c
    Crystal_Clear_funcs_22.c
    Crystal_Clear_funcs_23.c
    Crystal_Clear_funcs_24.c
    Crystal_Clear_funcs_25.c
    Crystal_Clear_funcs_26.c
    Crystal_Clear_funcs_27.c
    Crystal_Clear_funcs_28.c
    Crystal_Clear_funcs_29.c
    Crystal_Clear_funcs_30.c
    Crystal_Clear_funcs_31.c
    Crystal_Clear_funcs_32.c
    Crystal_Clear_funcs_33.c
    Crystal_Clear_funcs_34.c
    Crystal_Clear_funcs_35.c
    Crystal_Clear_funcs_36.c
    Crystal_Clear_funcs_37.c
    Crystal_Clear_funcs_38.c
    Crystal_Clear_funcs_39.c
    Crystal_Clear_funcs_40.c
    Crystal_Clear_funcs_41.c
    Crystal_Clear_funcs_42.c
    Crystal_Clear_funcs_43.c
    Crystal_Clear_funcs_44.c
    Crystal_Clear_funcs_45.c
    Crystal_Clear_funcs_46.c
    Crystal_Clear_funcs_47.c
    Crystal_Clear_funcs_48.c
    Crystal_Clear_funcs_49.c
    Crystal_Clear_funcs_50.c
    Crystal_Clear_funcs_51.c
    Crystal_Clear_funcs_52.c
    Crystal_Clear_funcs_53.c
    Crystal_Clear_funcs_54.c
    Crystal_Clear_funcs_55.c
    Crystal_Clear_funcs_56.c
    Crystal_Clear_funcs_57.c
    Crystal_Clear_funcs_58.c
    Crystal_Clear_funcs_59.c
    Crystal_Clear_funcs_60.c
    Crystal_Clear_funcs_61.c
    Crystal_Clear_funcs_62.c
    Crystal_Clear_funcs_63.c
    Crystal_Clear_funcs_64.c
    Crystal_Clear_funcs_65.c
    Crystal_Clear_funcs_66.c
    Crystal_Clear_funcs_67.c
    Crystal_Clear_funcs_68.c
)
set_source_files_properties(${GBRECOMP_GENERATED_SOURCES} PROPERTIES COMPILE_OPTIONS "-O${GBRECOMP_GENERATED_OPT_LEVEL}")

if(NOT MSVC AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(APPLE)
        target_link_options(gbrt PRIVATE -Wl,-dead_strip)
        target_link_options(Crystal_Clear PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(gbrt PRIVATE -Wl,--gc-sections)
        target_link_options(Crystal_Clear PRIVATE -Wl,--gc-sections)
    endif()
endif()

if(GBRECOMP_ENABLE_IPO AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    check_ipo_supported(RESULT GBRECOMP_IPO_SUPPORTED OUTPUT GBRECOMP_IPO_ERROR)
    if(GBRECOMP_IPO_SUPPORTED)
        set_property(TARGET gbrt PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        set_property(TARGET Crystal_Clear PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "GB Recompiled: IPO/LTO not enabled (${GBRECOMP_IPO_ERROR})")
    endif()
endif()

target_link_libraries(Crystal_Clear gbrt)
if(GBRECOMP_ENABLE_STRIP AND NOT MSVC AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(APPLE)
        add_custom_command(TARGET Crystal_Clear POST_BUILD
            COMMAND ${CMAKE_STRIP} -Sx $<TARGET_FILE:Crystal_Clear>
            VERBATIM)
    elseif(CMAKE_STRIP)
        add_custom_command(TARGET Crystal_Clear POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:Crystal_Clear>
            VERBATIM)
    endif()
endif()

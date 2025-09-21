cmake_minimum_required(VERSION 3.27)
project(aoc_c23 C)

# ---- Warnings
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wno-unused-function)
endif()

add_compile_definitions(
  COMPILER_CLANG=$<IF:$<C_COMPILER_ID:Clang>,1,0>
  COMPILER_MSVC=$<IF:$<C_COMPILER_ID:MSVC>,1,0>
  COMPILER_GCC=$<IF:$<C_COMPILER_ID:GNU>,1,0>
  COMPILER_CLANG_CL=$<IF:$<AND:$<C_COMPILER_ID:Clang>,$<BOOL:${MSVC}>>,1,0>
)

# ---- Select C23 explicitly per compiler
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if (MSVC)  # clang-cl (MSVC driver)
    add_compile_options(/clang:-std=c23)
  else()     # clang (gcc/clang driver)
    add_compile_options(-std=c23)
    add_compile_options(/clang:-Wno-unused-function)
  endif()
elseif (MSVC)
  # MSVC (cl.exe) doesn't support C23; if you build with MSVC, use C17
  add_compile_options(/std:c17)
endif()

# ---- Find each Day's main with relative paths
file(GLOB_RECURSE DAY_MAINS
     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
     CONFIGURE_DEPENDS
     "*/Day*/main.c")

# --- Feature Toggles #defines
option(PROJ_INTERNAL "Enable internal build-only features" OFF)
option(PROJ_SLOW    "Enable intentionally slow code paths" OFF)

# Debug-like configs default to ON
add_compile_definitions(
  PROJ_INTERNAL=$<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>,$<BOOL:${PROJ_INTERNAL}>>,1,0>
  PROJ_SLOW=$<IF:$<OR:$<CONFIG:Debug>,$<BOOL:${PROJ_SLOW}>>,1,0>
)

# ---- platform defines resolved at configure time
if (WIN32)
  add_compile_definitions(PROJ_WIN32=1 PROJ_LINUX=0 PROJ_MAC=0)
elseif(APPLE)
  add_compile_definitions(PROJ_WIN32=0 PROJ_LINUX=0 PROJ_MAC=1)
elseif(UNIX)
  add_compile_definitions(PROJ_WIN32=0 PROJ_LINUX=1 PROJ_MAC=0)
else()
  add_compile_definitions(PROJ_WIN32=0 PROJ_LINUX=0 PROJ_MAC=0)
endif()

foreach(src ${DAY_MAINS})
  get_filename_component(daydir "${src}" DIRECTORY)        # e.g., 2019/Day1
  get_filename_component(daydir_abs "${daydir}" ABSOLUTE)
  string(REPLACE "/" "_" target "${daydir}")               # e.g., 2019_Day1

  # file(GLOB DAY_SOURCES CONFIGURE_DEPENDS "${daydir}/main.c")
  # add_executable(${target} ${DAY_SOURCES})
  # Only build main.c for this day; it will #include all other .c chunks itself
  add_executable(${target} "${src}")

  # Put exe/pdb next to sources and inputs; run from that folder
  set(outdir "${daydir_abs}")
  set_target_properties(${target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${outdir}"
    PDB_OUTPUT_DIRECTORY "${outdir}"
    COMPILE_PDB_OUTPUT_DIRECTORY "${outdir}"
    VS_DEBUGGER_WORKING_DIRECTORY "${daydir_abs}"
  )
  foreach(cfg Debug Release RelWithDebInfo MinSizeRel)
    set_property(TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY_${cfg} "${outdir}")
    set_property(TARGET ${target} PROPERTY PDB_OUTPUT_DIRECTORY_${cfg} "${outdir}")
    set_property(TARGET ${target} PROPERTY COMPILE_PDB_OUTPUT_DIRECTORY_${cfg} "${outdir}")
    if (WIN32)
        target_link_libraries(${target} PRIVATE wininet)
    endif()
  endforeach()
endforeach()

# Which compiler did we get?
message(STATUS "C compiler: ${CMAKE_C_COMPILER} [ID=${CMAKE_C_COMPILER_ID}]")

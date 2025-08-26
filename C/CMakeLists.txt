cmake_minimum_required(VERSION 3.27)
project(aoc_c23 C)

# ---- Warnings
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow)
endif()

# ---- Select C23 explicitly per compiler
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if (MSVC)  # clang-cl (MSVC driver)
    add_compile_options(/clang:-std=c23)
  else()     # clang (gcc/clang driver)
    add_compile_options(-std=c23)
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

foreach(src ${DAY_MAINS})
  get_filename_component(daydir "${src}" DIRECTORY)        # e.g., 2019/Day1
  get_filename_component(daydir_abs "${daydir}" ABSOLUTE)
  string(REPLACE "/" "_" target "${daydir}")               # e.g., 2019_Day1

  file(GLOB DAY_SOURCES CONFIGURE_DEPENDS "${daydir}/*.c")
  add_executable(${target} ${DAY_SOURCES})

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
  endforeach()
endforeach()

# Which compiler did we get?
message(STATUS "C compiler: ${CMAKE_C_COMPILER} [ID=${CMAKE_C_COMPILER_ID}]")

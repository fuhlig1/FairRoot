################################################################################
# Copyright (C) 2014-2024 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  #
#                                                                              #
#              This software is distributed under the terms of the             #
#              GNU Lesser General Public Licence (LGPL) version 3,             #
#                  copied verbatim in the file "LICENSE"                       #
################################################################################

set(target MbsAPI)

set(sources
  f_evt.c
  f_stccomm.c
  f_ut_utime.c
  fLmd.c
)

fair_change_extensions_if_exists(.c .h FILES "${sources}" OUTVAR headers)

list(APPEND headers
  portnum_def.h
  s_bufhe_swap.h
  s_bufhe.h
  s_evhe_swap.h
  s_evhe.h
  s_filhe_swap.h
  s_filhe.h
  s_stdint.h
  s_ve10_1_swap.h
  s_ve10_1.h
  s_ves10_1_swap.h
  s_ves10_1.h
  sMbs.h
  typedefs.h
)

add_library(${target} SHARED ${sources} ${headers})
fairroot_library_settings(${target})

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${target} PRIVATE
    "-Wno-stringop-overflow"
    "-Wno-deprecated-declarations"
  )
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
  target_compile_options(${target} PRIVATE
    "-Wno-deprecated-non-prototype"
  )
endif()

target_include_directories(${target} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

check_symbol_exists(CLOCK_REALTIME "time.h" HAS_CLOCK_REALTIME)
target_compile_definitions(${target} PRIVATE
  $<$<PLATFORM_ID:Linux>:Linux>
  $<$<PLATFORM_ID:Linux>:SYSTEM64>
  $<$<PLATFORM_ID:Linux>:_LARGEFILE64_SOURCE>
  $<$<PLATFORM_ID:Darwin>:Darwin>
  $<$<PLATFORM_ID:Darwin>:SYSTEM64>
  $<$<PLATFORM_ID:Darwin>:_LARGEFILE64_SOURCE>
  $<$<BOOL:${HAS_CLOCK_REALTIME}>:_HAS_CLOCK_REALTIME>
)

fairroot_install_exported(TARGETS ${target})
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

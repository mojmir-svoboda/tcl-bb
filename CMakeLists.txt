project(bbTcl)

set(bbTcl_SOURCES
  tcl-bb.cpp
  bbcontroller.cpp
  callbacks.cpp
  dbroam.cpp
  dynabroam.cpp
  variables.cpp
  windowmessages.cpp
)

add_definitions(-DTCL4BB_EXPORTS)
add_definitions(-DSTDC_HEADERS)
add_definitions(-DTCL_THREADS=1)
add_definitions(-DUSE_THREAD_ALLOC=1)
add_definitions(-DSTATIC_BUILD)
add_definitions(-DTCL_CFGVAL_ENCODING="cp1252")

include_directories(${CMAKE_SOURCE_DIR}/blackbox)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/plugins/bbPlugin)
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/tcl8.6.4/generic)
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/tcl8.6.4/win)

add_library(bbTcl SHARED ${bbTcl_SOURCES} ${bbTcl_RESOURCES})
set_property(TARGET bbTcl PROPERTY FOLDER "plugins/bbTcl")
target_link_libraries(bbTcl blackbox bblib bbPlugin)
target_link_libraries(bbTcl version comctl32 user32 shell32 advapi32 netapi32 winmm)
target_link_libraries(bbTcl tcl tclstub)

install(TARGETS bbTcl RUNTIME DESTINATION plugins/bbTcl)

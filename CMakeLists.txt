#INSTALL_FILES = $(BIN) -as bbLeanBar+.txt readme.txt
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
include_directories(${CMAKE_SOURCE_DIR}/blackbox)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/plugins/bbPlugin)
add_library(bbTcl SHARED ${bbTcl_SOURCES} ${bbTcl_RESOURCES})
target_link_libraries(bbTcl blackbox bblib bbPlugin)
target_link_libraries(bbTcl version comctl32 user32 shell32 advapi32 winmm)

set_property(TARGET bbTcl PROPERTY FOLDER "plugins/bbTcl")

install(TARGETS bbTcl RUNTIME DESTINATION plugins/bbTcl)

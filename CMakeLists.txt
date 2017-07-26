cmake_minimum_required(VERSION 2.8)

# version number
set (GRAAI_VERSION_MAJOR 1)
set (GRAAI_VERSION_MINOR 0)

set (PROJECT_ROOT "${PROJECT_SOURCE_DIR}")
set (PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set (PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/bin")
set (PROJECT_CONFIG_DIR "${PROJECT_ROOT}/cfg")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


# configure header file to pass some of the cmake settings
# to the source files
configure_file (
  "${PROJECT_SOURCE_DIR}/GRAAI.hh.in"
  "${PROJECT_BINARY_DIR}/GRAAI.hh"
  )

# add binary tree to search path for include files
# so that we find GRAAI.hh
include_directories("${PROJECT_BINARY_DIR}" 
                    "${PROJECT_SOURCE_DIR}")

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBMODBUS REQUIRED libmodbus)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++1y -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set (GRAAI_SOURCES ${PROJECT_SOURCE_DIR}/GRAAI.cc 
                   ${PROJECT_SOURCE_DIR}/TagEngine.cc 
                   ${PROJECT_SOURCE_DIR}/ModbusPkg.cc 
                   ${PROJECT_SOURCE_DIR}/ModbusServer.cc)

add_executable (GRAAI ${GRAAI_SOURCES})

#link libmodbus
target_link_libraries(GRAAI ${LIBMODBUS_LIBRARIES})
target_include_directories(GRAAI PUBLIC ${LIBMODBUS_INCLUDE_DIRS})
target_compile_options(GRAAI PUBLIC ${LIBMODBUS_CFLAGS_OTHER})

#add the install targets
install (TARGETS GRAAI 
         DESTINATION bin)

install (FILES "${PROJECT_BINARY_DIR}/GRAAI.hh"        
               "${PROJECT_BINARY_DIR}/ModbusPlugin.hh"
               "${PROJECT_BINARY_DIR}/TagEngine.hh"
         DESTINATION include)



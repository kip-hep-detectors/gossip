cmake_minimum_required(VERSION 2.8)

project(gossip)

#Set version number
set(gossip_VERSION_MAJOR 1)
set(gossip_VERSION_MINOR 32)
set(gossip_VERSION ${gossip_VERSION_MAJOR}.${gossip_VERSION_MINOR})

configure_file(
	"${gossip_SOURCE_DIR}/cmake/gossipConfig.h.in"
	"${gossip_BINARY_DIR}/include/gossipConfig.h"
	)

# set default build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(EXTRA_OPTIONS "-03")

# add CERN ROOT
find_package(ROOT 6 REQUIRED)
include(${ROOT_USE_FILE})

include_directories(${ROOT_INCLUDE_DIRS} ${gossip_SOURCE_DIR}/include ${gossip_BINARY_DIR}/include)

file(GLOB gossip_lib_SOURCES "src/*.cpp")
file(GLOB gossip_lib_HEADERS "include/*.h")
list(REMOVE_ITEM gossip_lib_HEADERS "${gossip_SOURCE_DIR}/include/LinkDef.h")

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set library / binary output directories
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

root_generate_dictionary(gossip ${gossip_lib_HEADERS} LINKDEF ${gossip_SOURCE_DIR}/include/LinkDef.h)

# build the library
add_library(gossip_lib SHARED ${gossip_lib_SOURCES} gossip.cxx)
set_target_properties(gossip_lib PROPERTIES OUTPUT_NAME gossip)
target_link_libraries(gossip_lib ${ROOT_LIBRARIES} Gui)

# build the executable
add_executable(gossip_exe gossip.cpp)
set_target_properties(gossip_exe PROPERTIES OUTPUT_NAME gossip)
target_link_libraries(gossip_exe gossip_lib ${ROOT_LIBRARIES} Rint)


# Add all targets to the build-tree export set
export(TARGETS gossip_lib
	FILE "${PROJECT_BINARY_DIR}/gossipTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE gossip)

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/include")
set(CONF_LIBS "${PROJECT_BINARY_DIR}/lib/libgossip.so")
configure_file(cmake/gossipConfig.cmake.in
	"${PROJECT_BINARY_DIR}/gossipConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
set(CONF_LIBS "${CMAKE_INSTALL_PREFIX}/lib/libgossip.so")
configure_file(cmake/gossipConfig.cmake.in
	"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/gossipConfig.cmake" @ONLY)

# ... for both
configure_file(cmake/gossipConfigVersion.cmake.in
	"${PROJECT_BINARY_DIR}/gossipConfigVersion.cmake" @ONLY)

# copy header files to build dir
add_custom_command(TARGET gossip_lib PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)

# copy rdict and rootmap to build dir
add_custom_command(TARGET gossip_lib POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_BINARY_DIR}/gossip_rdict.pcm ${PROJECT_BINARY_DIR}/lib)

add_custom_command(TARGET gossip_lib POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_BINARY_DIR}/libgossip.rootmap ${PROJECT_BINARY_DIR}/lib)


# install target
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/gossipConfig.cmake" "${PROJECT_BINARY_DIR}/gossipConfigVersion.cmake"
	DESTINATION cmake
	COMPONENT dev)

install(FILES ${gossip_lib_HEADERS}
	DESTINATION include
	COMPONENT dev)

install(FILES ${gossip_BINARY_DIR}/lib/gossip_rdict.pcm ${gossip_BINARY_DIR}/lib/libgossip.rootmap
	DESTINATION lib)

install(TARGETS gossip_lib
	EXPORT gossip-targets
	DESTINATION lib
	COMPONENT shlib)

install(TARGETS gossip_exe
	DESTINATION bin)


# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


cmake_minimum_required(VERSION 2.8)

project(gossip_example1)

# add Device-library
find_package(gossip 1.3 REQUIRED)

# add CERN ROOT
find_package(ROOT 6 REQUIRED)
include(${ROOT_USE_FILE})

# set include directories
include_directories(${ROOT_INCLUDE_DIRS} ${gossip_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

#set RPATH variables
set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# build executable
add_executable(gossip_example1_exe src/example1.cpp)
set_target_properties(gossip_example1_exe PROPERTIES OUTPUT_NAME example1)
target_link_libraries(gossip_example1_exe ${ROOT_LIBRARIES} ${gossip_LIBRARIES})


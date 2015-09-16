cmake_minimum_required(VERSION 2.8)

project(gossip)

### add CERN ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

include_directories(${ROOT_INCLUDE_DIRS} ${gossip_SOURCE_DIR}/include)

#set(gossip_VERSION 1.25)
set(gossip_lib_SOURCES ${gossip_SOURCE_DIR}/src/daqMC.cpp
                       ${gossip_SOURCE_DIR}/src/gossipGUI.cpp
                       ${gossip_SOURCE_DIR}/src/HitMatrix.cpp
                       ${gossip_SOURCE_DIR}/src/PhotonList.cpp
                       ${gossip_SOURCE_DIR}/src/PhotonSource.cpp
                       ${gossip_SOURCE_DIR}/src/sipmMC.cpp)

set(gossip_lib_HEADERS ${gossip_SOURCE_DIR}/include/daqMC.h
                       ${gossip_SOURCE_DIR}/include/gossipGUI.h
                       ${gossip_SOURCE_DIR}/include/HitMatrix.h
                       ${gossip_SOURCE_DIR}/include/PhotonList.h
                       ${gossip_SOURCE_DIR}/include/PhotonSource.h
                       ${gossip_SOURCE_DIR}/include/sipmMC.h)

root_generate_dictionary(Dict ${gossip_lib_HEADERS} LINKDEF ${gossip_SOURCE_DIR}/include/LinkDef.h)

# build the library
add_library(gossip_lib SHARED ${gossip_lib_SOURCES} Dict.cxx)
set_target_properties(gossip_lib PROPERTIES OUTPUT_NAME gossip)
target_link_libraries(gossip_lib ${ROOT_GUI_LIBRARIES})

# build the executable
add_executable(gossip_exe main.cpp)
set_target_properties(gossip_exe PROPERTIES OUTPUT_NAME gossip)
target_link_libraries(gossip_exe gossip_lib ${ROOT_GUI_LIBRARIES} Gui Rint)

#install(FILES ${gossip_lib_HEADERS} DESTINATION include/gossip)
#install(TARGETS gossip_lib EXPORT gossip-targets DESTINATION lib)
#install(TARGETS gossip_exe DESTINATION bin)

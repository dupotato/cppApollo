cmake_minimum_required(VERSION 3.14)
#project(apollo CXX C)
project(apolloclient)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -std=c++11")  #设置c++的编译选项
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive  -std=c99")  #设置c的编译选项
set(CMAKE_C_STANDARD 99)
set(CMAKE_SOURCE_DIR ./)

include_directories(./ ./libs/jsoncpp/include )
aux_source_directory(${CMAKE_SOURCE_DIR} dir)

LINK_DIRECTORIES(./libs/jsoncpp/lib)
add_executable(apolloclient ${dir})
target_link_libraries(apolloclient json curl pthread ssl crypto)


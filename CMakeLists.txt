cmake_minimum_required(VERSION 2.8)
project(yoda-sixsix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(deps/libuv-cmake/libuv/include)
include_directories(deps/ini-reader)
include_directories(deps/rapidjson)
include_directories(deps/libwebsockets/)
include_directories(deps/caps/)
include_directories(./src/)
include_directories(./src/message/)
include_directories(./src/executors/)
include_directories(./src/websocket/)

add_subdirectory(deps/libuv-cmake)
add_subdirectory(deps/libwebsockets)
add_subdirectory(deps/caps)

aux_source_directory(src SRC_LIST)
aux_source_directory(src/executors EXECUTOR_LIST)
aux_source_directory(src/websocket WEBSOCKETCLIENT)
aux_source_directory(src/message CAPS_MESSAGE)

add_executable(yoda-sixsix ${SRC_LIST} ${EXECUTOR_LIST} ${WEBSOCKETCLIENT} ${CAPS_MESSAGE})
target_link_libraries(yoda-sixsix uv libwebsockets caps)

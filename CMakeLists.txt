PROJECT(SunShine)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "This is BINARY dir " ${SunShine_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${SunShine_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SunShine_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SunShine_BINARY_DIR}/lib) 

ADD_SUBDIRECTORY(lib/common)
ADD_SUBDIRECTORY(lib/net)
ADD_SUBDIRECTORY(lib/proto)
ADD_SUBDIRECTORY(lib/service)
ADD_SUBDIRECTORY(lib/json)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(test_json)
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(gateway)
ADD_SUBDIRECTORY(master)
ADD_SUBDIRECTORY(scene_server)
ADD_SUBDIRECTORY(data_server)
ADD_SUBDIRECTORY(test_server)


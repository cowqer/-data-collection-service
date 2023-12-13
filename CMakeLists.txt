cmake_minimum_required(VERSION 3.5)
project(HDF5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH OFF)
set(CMAKE_FIND_USE_SYSTEM_INCLUDE_PATH OFF)
set(CMAKE_FIND_USE_SYSTEM_LIBRARY_PATH OFF)

set(PLATFORM "amd64")  
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(PLATFORM "aarch64")  
    message("Detected aarch64 architecture")
else()
    message("Not aarch64 architecture")
endif()

set(MQTT_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/mqtt/${PLATFORM}")
set(HDF5_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/hdf5/${PLATFORM}")
set(OPENCV_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/opencv/${PLATFORM}")
set(ETC_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/etc")
set(X264_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/x264/${PLATFORM}")
set(BOOST_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")

find_path(BOOST_INCLUDE_DIRS NAMES boost/asio.hpp PATHS "${CMAKE_SOURCE_DIR}/include")
find_path(HDF5_INCLUDE_DIRS NAMES H5Cpp.h PATHS "${CMAKE_SOURCE_DIR}/include/hdf5")
find_path(OPENCV_INCLUDE_DIRS NAMES opencv2/opencv.hpp PATHS "${CMAKE_SOURCE_DIR}/include/opencv")
find_path(MQTT_INCLUDE_DIRS NAMES mqtt/async_client.h PATHS "${CMAKE_SOURCE_DIR}/include/mqtt")
find_path(JSON_INCLUDE_DIRS NAMES nlohmann/json.hpp PATHS "${CMAKE_SOURCE_DIR}/include/json")
find_path(X264_INCLUDE_DIRS NAMES x264.h PATHS "${CMAKE_SOURCE_DIR}/include/x264")

include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${MQTT_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${JSON_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${X264_INCLUDE_DIRS})

set(SRC_LIST
    src/main.cpp
    src/sensors/Camera.cpp
    src/sensors/Lidar.cpp
    src/sensors/Radar.cpp
    src/sensors/GPS.cpp
    src/sensors/IMU.cpp
    src/sensors/mqtt.cpp
    )

add_executable(coll ${SRC_LIST})
add_executable(pub0 src/pub_0.cpp)
add_executable(rec0 src/rec_0.cpp)

message(STATUS "PLATFORM: ${PLATFORM}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "HDF5_include_DIRS: ${HDF5_INCLUDE_DIRS}")
message(STATUS "MqttCpp_Include_DIRS: ${MQTT_INCLUDE_DIRS}")
message(STATUS "OpenCV_Include_DIRS: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "JSON_include_DIRS: ${JSON_INCLUDE_DIRS}")
message(STATUS "BOOST_Include_DIRS: ${BOOST_INCLUDE_DIRS}")
message(STATUS "x264_Include: ${X264_INCLUDE_DIRS}!!")

message(STATUS "ETC_LIBS: ${ETC_LIBRARY_PATH}")
message(STATUS "OpenCV_LIB: ${OPENCV_LIBRARY_PATH}")
message(STATUS "HDF5_LIB: ${HDF5_LIBRARY_PATH}")
message(STATUS "MqttCpp_LIB: ${MQTT_LIBRARY_PATH}")

link_directories(
    ${HDF5_LIBRARY_PATH}
    ${MQTT_LIBRARY_PATH}
    ${OPENCV_LIBRARY_PATH}
    
)

set(COMMON_LIBRARIES
    -lpaho-mqttpp3
    -lpaho-mqtt3as
    -lpaho-mqtt3c
)

target_link_libraries(pub0
    -L${MQTT_LIBRARY_PATH}
    ${COMMON_LIBRARIES}
)

target_link_libraries(rec0
    -L${MQTT_LIBRARY_PATH}
    ${COMMON_LIBRARIES}
)
#amd64下需要添加
target_link_libraries(coll PRIVATE 
    -L${HDF5_LIBRARY_PATH}
    -L${MQTT_LIBRARY_PATH}
    -L${OPENCV_LIBRARY_PATH}
    -L${X264_LIBRARY_PATH}
    -lopencv_imgproc
    -lopencv_core
    -lopencv_imgcodecs
    -lopencv_highgui
    -lopencv_videoio
    ${COMMON_LIBRARIES}
    -lz
    -lsz
    -lhdf5
    -lhdf5_cpp
    -lcrypto
    -lpthread
    -lx264
)


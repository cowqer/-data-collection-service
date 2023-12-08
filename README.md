# DATE_COLLECTION of FUSION （AARCH64）

## Getting started
```
mkdir build 
cd build
cmake & make
./main
#or 
./runtest.sh
```
To make it easy for you to get started with GitLab, here's a list of recommended next steps.

Already a pro? Just edit this README.md and make it your own. Want to make it easy? [Use the template at the bottom](#editing-this-readme)!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Add to environment   please modify the path to the really path of your device 
## !!!
export LD_LIBRARY_PATH=/home/love32/Desktop/Database/data-collection-service/lib/mqtt/linux_amd64:/lib/mqtt/linux_amd64:/home/love32/Desktop/Database/data-collection-service/lib/hdf5/lib_amd64:/home/love32/Desktop/Database/data-collection-service/lib/etc:$LD_LIBRARY_PATH
## IF ERROR: no found libpthread_nonshared.a(choose one of below commads):
```
ar cr /usr/lib64/libpthread_nonshared.a
ln -s libpthread.a libpthread_nonshared.a
```
## Cross Compile Command 
```
arm-linux-gnueabihf-gcc -I/path/to/your/include -L/path/to/your/lib -lyourlibrary your_source.c -o /path/to/your/executable/your_executable_name


arm-linux-gnueabihf-gcc -I./include/mqtt -I./include/openssl -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttc/lib -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttcpp ./src/pub_0.cpp -o ./arm_test/pub0 -lpaho-mqttpp3 -lpaho-mqtt3as -lpaho-mqtt3c -lssl -lcrypto -lstdc++

arm-linux-gnueabihf-gcc \
  -I./include/mqtt \
  -I./include/openssl \
  -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttc/lib \
  -L/home/cq/Desktop/git/data-collection-service/lib/mqtt/arrch64/mqttcpp \
  ./src/pub_0.cpp \
  -o ./arm_test/pub0 \
  -lpaho-mqttpp3 \
  -lpaho-mqtt3as \
  -lpaho-mqtt3c \
  -lssl \
  -lcrypto \
  -lstdc++



./config no-asm shared no-async --prefix=/home/cq/Desktio/arm_openssl --cross-compile-prefix=arm-linux-gnueabihf

sudo chmod 777 client

```
## Add your files

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
cd existing_repo
git remote add origin http://fusion-dc.net:5080/data-center/data-collection-service.git
git branch -M main
git push -uf origin main
```

## Integrate with your tools

- [ ] [Set up project integrations](http://fusion-dc.net:5080/data-center/data-collection-service/-/settings/integrations)

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Set auto-merge](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***

# Editing this README

When you're ready to make this README your own, just edit this file and use the handy template below (or feel free to structure it however you want - this is just a starting point!). Thank you to [makeareadme.com](https://www.makeareadme.com/) for this template.

## Suggestions for a good README
Every project is different, so consider which of these sections apply to yours. The sections used in the template are suggestions for most open source projects. Also keep in mind that while a README can be too long and detailed, too long is better than too short. If you think your README is too long, consider utilizing another form of documentation rather than cutting out information.

## Name
Choose a self-explaining name for your project.

## Description
Let people know what your project can do specifically. Provide context and add a link to any reference visitors might be unfamiliar with. A list of Features or a Background subsection can also be added here. If there are alternatives to your project, this is a good place to list differentiating factors.

## Badges
On some READMEs, you may see small images that convey metadata, such as whether or not all the tests are passing for the project. You can use Shields to add some to your README. Many services also have instructions for adding a badge.

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment


## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.


## backup
cmake_minimum_required(VERSION 3.5)
project(HDF5)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.2  # 或者你选择的版本
)
FetchContent_MakeAvailable(json)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)
# 禁用系统环境路径
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH OFF)
# 禁用系统库和头文件路径
set(CMAKE_FIND_USE_SYSTEM_INCLUDE_PATH OFF)
set(CMAKE_FIND_USE_SYSTEM_LIBRARY_PATH OFF)

set(PLATFORM "amd64")  # 设置默认平台为amd64
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(PLATFORM "aarch64")  # 如果是aarch64平台，则覆盖默认值为aarch64

endif()

set(MQTT_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/mqtt/${PLATFORM}")
set(HDF5_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/hdf5/${PLATFORM}")
set(OPENCV_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/opencv/${PLATFORM}")

find_path(HDF5_INCLUDE_DIRS NAMES H5Cpp.h PATHS "${CMAKE_SOURCE_DIR}/include/hdf5")
find_path(OPENCV_INCLUDE_DIRS NAMES opencv2/opencv.hpp PATHS "${CMAKE_SOURCE_DIR}/include/opencv")
find_path(MQTT_INCLUDE_DIRS NAMES mqtt/async_client.h PATHS "${CMAKE_SOURCE_DIR}/include/mqtt")

include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${MQTT_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})

set(SRC_LIST
    src/main.cpp
    src/sensors/Camera.cpp
    src/sensors/Lidar.cpp
    src/sensors/Radar.cpp
    src/sensors/GPS.cpp
    src/sensors/IMU.cpp
    src/sensors/mqtt.cpp
    )

add_executable(hdf5 ${SRC_LIST})
add_executable(pub0 src/pub_0.cpp)
add_executable(rec0 src/rec_0.cpp)
add_executable(cv_test src/opencv_test.cpp)

#message(STATUS "HDF5_LIBRARIES22: ${HDF5_LIBRARIES}")
message(STATUS "OpenCV_Include_DIRS: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_Libraries: ${OPENCV_LIBRARY_PATH}")
message(STATUS "HDF5_include_DIRS: ${HDF5_INCLUDE_DIRS}")
message(STATUS "HDF5_LIBRARIES: ${HDF5_LIBRARY_PATH}")
message(STATUS "MqttCpp_Include: ${MQTT_INCLUDE_DIRS}!")
message(STATUS "MqttCpp_Libraries: ${MQTT_LIBRARY_PATH}")



target_link_libraries(pub0
    -L${MQTT_LIBRARY_PATH}
    -lpaho-mqttpp3
    -lpaho-mqtt3as
    )
target_link_libraries(rec0
    -L${MQTT_LIBRARY_PATH}
    -lpaho-mqttpp3
    -lpaho-mqtt3as
    )

target_link_libraries(hdf5 
    -L${HDF5_LIBRARY_PATH}
    -L${MQTT_LIBRARY_PATH}
    -L${OPENCV_LIBRARY_PATH}
    -lpaho-mqttpp3
    -lpaho-mqtt3as 
    opencv_imgproc
    opencv_core
    opencv_imgcodecs
    opencv_highgui
    -lopencv_videoio
    -lhdf5_cpp
    -lhdf5_serial 
    -pthread
    ${OPENCV_LIBRARIES}
)


target_link_libraries(cv_test
    -L${OPENCV_LIBRARY_PATH}
    opencv_imgproc
    opencv_core
    opencv_imgcodecs
    opencv_highgui
    -lopencv_videoio
)

target_include_directories(hdf5 PRIVATE ${json_SOURCE_DIR}/include)

 /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_cpp.so;
 /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.so;
 /usr/local/lib/libcrypto.so;
 /usr/lib/x86_64-linux-gnu/libcurl.so;
 /usr/lib/x86_64-linux-gnu/libpthread.a;
 /usr/lib/x86_64-linux-gnu/libsz.so;
 /usr/lib/x86_64-linux-gnu/libz.so;
 /usr/lib/x86_64-linux-gnu/libdl.a;
 /usr/lib/x86_64-linux-gnu/libm.so
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


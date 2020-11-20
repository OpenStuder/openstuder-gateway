cmake_minimum_required(VERSION 3.16)
project(StuderGateway)
find_package(Qt5 COMPONENTS Core REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(SIDeviceAccess
            src/api/sistatus.h
            src/api/sistatus.cpp
            src/api/sipropertyid.h
            src/api/sipropertytype.h
            src/api/sipropertytype.cpp
            src/api/sipropertyflags.h
            src/api/sipropertyreadresult.h
            src/api/sipropertywriteresult.h
            src/api/sidevice.h
            src/api/sijsonflags.h
            src/api/sidevice.cpp
            src/api/sideviceaccess.h
            src/api/sideviceaccess.cpp
            src/api/sideviceaccessregistry.h
            src/api/sideviceaccessregistry.cpp
            )
target_include_directories(SIDeviceAccess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/api)
target_link_libraries(SIDeviceAccess Qt5::Core)

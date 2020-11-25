cmake_minimum_required(VERSION 3.16)
project(StuderGateway)
find_package(Qt5 COMPONENTS Core SerialPort SerialBus REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(SIDeviceAccess SHARED
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
            src/api/sideviceaccessdriver.h
            )
target_include_directories(SIDeviceAccess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/api)
target_link_libraries(SIDeviceAccess Qt5::Core)

function(add_device_access_driver name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} SIDeviceAccess)
    set_target_properties(${name} PROPERTIES
                          PREFIX ""
                          SUFFIX ".dap"
                          )
endfunction()

add_device_access_driver(XCom485i
                         src/drivers/xcom485i/xcom485idevice.h
                         src/drivers/xcom485i/xcom485idevice.cpp
                         src/drivers/xcom485i/xcom485ideviceaccess.h
                         src/drivers/xcom485i/xcom485ideviceaccess.cpp
                         src/drivers/xcom485i/xcom485idriver.h
                         src/drivers/xcom485i/xcom485idriver.cpp
                         src/drivers/xcom485i/xcom485ixtender.h
                         src/drivers/xcom485i/xcom485ixtender.cpp
                         src/drivers/xcom485i/xcom485ivariotrack.h
                         src/drivers/xcom485i/xcom485ivariotrack.cpp
                         src/drivers/xcom485i/xcom485ivariostring.h
                         src/drivers/xcom485i/xcom485ivariostring.cpp
                         )

target_link_libraries(XCom485i Qt5::SerialPort Qt5::SerialBus)


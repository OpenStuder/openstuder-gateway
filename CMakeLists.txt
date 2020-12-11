cmake_minimum_required(VERSION 3.16)
project(StuderGateway)
find_package(Qt5 COMPONENTS Core SerialPort SerialBus Sql Network WebSockets REQUIRED)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(SIDeviceAccess SHARED
            src/api/deviceaccess/sistatus.h
            src/api/deviceaccess/sistatus.cpp
            src/api/deviceaccess/sipropertyid.h
            src/api/deviceaccess/sipropertytype.h
            src/api/deviceaccess/sipropertytype.cpp
            src/api/deviceaccess/sipropertyflags.h
            src/api/deviceaccess/sipropertyreadresult.h
            src/api/deviceaccess/sipropertywriteflags.h
            src/api/deviceaccess/sipropertywriteresult.h
            src/api/deviceaccess/sidevice.h
            src/api/deviceaccess/sijsonflags.h
            src/api/deviceaccess/sidevice.cpp
            src/api/deviceaccess/sideviceaccess.h
            src/api/deviceaccess/sideviceaccess.cpp
            src/api/deviceaccess/sideviceaccessregistry.h
            src/api/deviceaccess/sideviceaccessregistry.cpp
            src/api/deviceaccess/sideviceaccessdriver.h
            )
target_include_directories(SIDeviceAccess INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/api/deviceaccess)
target_link_libraries(SIDeviceAccess Qt5::Core)

add_library(SIStorage SHARED
            src/api/storage/sistorage.h
            src/api/storage/sipropertyhistorystorage.h
            src/api/storage/sipropertyhistorystorage.cpp
            src/api/storage/sistoragedriver.h
            src/api/storage/sistoragedriver.cpp
            )
target_include_directories(SIStorage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/api/storage)
target_link_libraries(SIStorage SIDeviceAccess Qt5::Core)

function(add_device_access_driver name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} SIDeviceAccess)
    set_target_properties(${name} PROPERTIES
                          PREFIX ""
                          SUFFIX ".sdd"
                          )
endfunction()

function(add_storage_driver name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} SIDeviceAccess SIStorage)
    set_target_properties(${name} PROPERTIES
                          PREFIX ""
                          SUFFIX ".ssd"
                          )
endfunction()

add_device_access_driver(XCom485i
                         src/drivers/deviceaccess/xcom485i/xcom485iproperty.h
                         src/drivers/deviceaccess/xcom485i/xcom485idevice.h
                         src/drivers/deviceaccess/xcom485i/xcom485idevice.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485ideviceaccess.h
                         src/drivers/deviceaccess/xcom485i/xcom485ideviceaccess.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485idriver.h
                         src/drivers/deviceaccess/xcom485i/xcom485idriver.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485ixtender.h
                         src/drivers/deviceaccess/xcom485i/xcom485ixtender.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485ivariotrack.h
                         src/drivers/deviceaccess/xcom485i/xcom485ivariotrack.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485ivariostring.h
                         src/drivers/deviceaccess/xcom485i/xcom485ivariostring.cpp
                         src/drivers/deviceaccess/xcom485i/xcom485ibatterymanager.h
                         src/drivers/deviceaccess/xcom485i/xcom485ibatterymanager.cpp
                         )
target_link_libraries(XCom485i Qt5::SerialPort Qt5::SerialBus)

add_storage_driver(SQLite
                   src/drivers/storage/sqlite/sqlitestorage.h
                   src/drivers/storage/sqlite/sqlitestorage.cpp
                   src/drivers/storage/sqlite/sqlitestoragedriver.h
                   src/drivers/storage/sqlite/sqlitestoragedriver.cpp
                   )
target_link_libraries(SQLite Qt5::Sql)

add_executable(SIDaemon
               src/daemon/main.cpp
               src/daemon/sidaemon.h
               src/daemon/sidaemon.cpp
               src/daemon/deviceaccess/sidevicepropertymanager.cpp
               src/daemon/deviceaccess/sidevicepropertymanager.h
               src/daemon/deviceaccess/siglobalpropertyid.cpp
               src/daemon/deviceaccess/siglobalpropertyid.h
               src/daemon/deviceaccess/operations/sipropertyreadoperation.cpp
               src/daemon/deviceaccess/operations/sipropertyreadoperation.h
               src/daemon/deviceaccess/operations/sipropertywriteoperation.cpp
               src/daemon/deviceaccess/operations/sipropertywriteoperation.h
               src/daemon/deviceaccess/operations/siabstractoperation.cpp
               src/daemon/deviceaccess/operations/siabstractoperation.h
               src/daemon/deviceaccess/sisequentialpropertymanager.cpp
               src/daemon/deviceaccess/sisequentialpropertymanager.h
               src/daemon/deviceaccess/operations/sideviceenumerationoperation.cpp
               src/daemon/deviceaccess/operations/sideviceenumerationoperation.h
               src/daemon/websocket/siwebsocketmanager.cpp
               src/daemon/websocket/siwebsocketmanager.h src/daemon/websocket/siwebsocketconnection.cpp src/daemon/websocket/siwebsocketconnection.h src/daemon/websocket/siprotocolframe.cpp src/daemon/websocket/siprotocolframe.h)
target_link_libraries(SIDaemon SIDeviceAccess SIStorage Qt5::Core Qt5::Network Qt5::WebSockets)

# Project settings.
cmake_minimum_required(VERSION 3.13)
project(OpenStuderGateway)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)
add_compile_definitions(OPENSTUDER_GATEWAY_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION="${OPENSTUDER_GATEWAY_INSTALL_PREFIX}/etc")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_DRIVER_LOCATION="${OPENSTUDER_GATEWAY_INSTALL_PREFIX}/var/lib/openstuder/drivers")

# Import gateway related functions.
include(cmake/library.cmake)
include(cmake/deviceaccessdriver.cmake)
include(cmake/storagedriver.cmake)

# Resolve required packages.
find_package(Qt5 5.11 REQUIRED COMPONENTS Core)

# Libraries.
add_subdirectory(src/api/deviceaccess)
add_subdirectory(src/api/storage)

# Executables.
add_subdirectory(src/daemon)

# Drivers.
add_subdirectory(src/drivers/storage/sqlite)
add_subdirectory(src/drivers/deviceaccess/xcom485i)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(src/drivers/deviceaccess/dummy)
endif()

# Configuration file.
install(FILES config/sigatewayd.conf DESTINATION "etc/openstuder")

# Install instructions.
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a,libqt5serialport5,libqt5serialbus5,libqt5sql5,libqt5network5,libqt5websockets5,libqt5connectivity5")
set(CPACK_PACKAGE_DESCRIPTION "OpenStuder Gateway")
include(CPack)

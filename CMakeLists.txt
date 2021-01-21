# Global project settings.
cmake_minimum_required(VERSION 3.13)
project(OpenStuderGateway)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr/" CACHE PATH "..." FORCE)
endif()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
add_compile_options(-Wall -Wextra -pedantic -Werror)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wno-psabi)
endif()

# Detect and set version.
include(cmake/GitVersion.cmake)
si_get_git_version(GIT_VERSION)

# Import gateway related functions.
include(cmake/Library.cmake)
include(cmake/DeviceAccessDriver.cmake)
include(cmake/StorageDriver.cmake)

# Resolve required packages.
find_package(Qt5 5.11 REQUIRED COMPONENTS Core)

# Set global compile definitions.
add_compile_definitions(OPENSTUDER_GATEWAY_VERSION="${GIT_VERSION}")
add_compile_definitions(OPENSTUDER_GATEWAY_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION="${CMAKE_INSTALL_PREFIX}/etc/openstuder")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_DRIVER_LOCATION="${CMAKE_INSTALL_PREFIX}/var/lib/openstuder/drivers")

# Libraries.
add_subdirectory(src/api/deviceaccess)
add_subdirectory(src/api/storage)
add_subdirectory(src/api/authorize)

# Executables.
add_subdirectory(src/daemon)
add_subdirectory(src/cli)

# Drivers.
add_subdirectory(src/drivers/storage/sqlite)
add_subdirectory(src/drivers/deviceaccess/xcom485i)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(src/drivers/deviceaccess/dummy)
endif()

# Configuration file.
install(FILES config/gateway.conf DESTINATION "etc/openstuder")
install(FILES config/drivers.conf DESTINATION "etc/openstuder")

# Service (systemd).
configure_file(config/sigatewayd.service.in ${CMAKE_BINARY_DIR}/sigatewayd.service @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/sigatewayd.service DESTINATION "/lib/systemd/system")

# Install instructions.
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION "OpenStuder Gateway")
set(CPACK_PACKAGE_VERSION ${GIT_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "michel.clausen@hevs.ch")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a,libqt5serialport5,libqt5serialbus5,libqt5sql5,libqt5network5,libqt5websockets5,libqt5bluetooth5")
include(CPack)

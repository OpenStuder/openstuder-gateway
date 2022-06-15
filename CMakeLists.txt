cmake_minimum_required(VERSION 3.13)
project(openstuder-gateway)

# Load cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(GNUInstallDirs)
include(SIGitVersion)
include(SILibrary)
include(SIDeviceAccessDriver)
include(SIStorageDriver)
include(SIExtensionDriver)

# Handle install prefix.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "..." FORCE)
endif()

# CMake settings.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Compile options.
add_compile_options(-Wall -Wextra -pedantic -Werror)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wno-psabi)
endif()

# Detect and set version from git repository.
si_get_git_version(OPENSTUDER_VERSION)

# Resolve required packages.
find_package(Qt5 5.11 REQUIRED COMPONENTS Core)

# Global variables.
set(OPENSTUDER_CONFIG_LOCATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/openstuder")
set(OPENSTUDER_DRIVER_LOCATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}/openstuder/drivers")
set(OPENSTUDER_SYSTEMD_UNIT_LOCATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/systemd/system")

# Set global compile definitions.
add_compile_definitions(OPENSTUDER_GATEWAY_VERSION="${OPENSTUDER_VERSION}")
add_compile_definitions(OPENSTUDER_GATEWAY_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION="${OPENSTUDER_CONFIG_LOCATION}")
add_compile_definitions(OPENSTUDER_GATEWAY_DEFAULT_DRIVER_LOCATION="${OPENSTUDER_DRIVER_LOCATION}")

# Libraries (SDK).
add_subdirectory(src/api/deviceaccess)
add_subdirectory(src/api/storage)
add_subdirectory(src/api/authorize)
add_subdirectory(src/api/extension)

# Executables.
add_subdirectory(src/sigatewayd)
add_subdirectory(src/sigwctl)

# Drivers.
add_subdirectory(src/drivers/storage/sqlite)
add_subdirectory(src/drivers/deviceaccess/xcom485i)
add_subdirectory(src/drivers/deviceaccess/demo)
add_subdirectory(src/drivers/extension/webstorage)

# Configuration files.
install(FILES config/gateway.conf DESTINATION "${OPENSTUDER_CONFIG_LOCATION}")
install(FILES config/drivers.conf DESTINATION "${OPENSTUDER_CONFIG_LOCATION}")
install(FILES config/datalog.conf DESTINATION "${OPENSTUDER_CONFIG_LOCATION}")

# Service (systemd).
configure_file(install/sigatewayd.service.in ${CMAKE_BINARY_DIR}/sigatewayd.service @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/sigatewayd.service DESTINATION ${OPENSTUDER_SYSTEMD_UNIT_LOCATION})

# Install instructions.
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/")
set(CPACK_PACKAGE_DESCRIPTION "openstuder gateway")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://openstuder.github.io")
set(CPACK_PACKAGE_VERSION ${OPENSTUDER_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "michel.clausen@hevs.ch")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/install/postinst;${CMAKE_CURRENT_SOURCE_DIR}/install/prerm;${CMAKE_CURRENT_SOURCE_DIR}/install/postrm" "${CMAKE_CURRENT_SOURCE_DIR}/install/conffiles")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a,libqt5serialport5,libqt5serialbus5,libqt5sql5,libqt5network5,libqt5websockets5,libqt5bluetooth5")
include(CPack)
find_package(Qt5 5.11 REQUIRED COMPONENTS Network WebSockets Bluetooth)

add_library(tinycbor STATIC
            bluetooth/tinycbor/cbor.h
            bluetooth/tinycbor/cborencoder.c
            bluetooth/tinycbor/cborencoder_close_container_checked.c
            bluetooth/tinycbor/cborerrorstrings.c
            bluetooth/tinycbor/cborinternal_p.h
            bluetooth/tinycbor/cborjson.h
            bluetooth/tinycbor/cborparser.c
            bluetooth/tinycbor/cborparser_dup_string.c
            bluetooth/tinycbor/cborpretty.c
            bluetooth/tinycbor/cborpretty_stdio.c
            bluetooth/tinycbor/cbortojson.c
            bluetooth/tinycbor/cborvalidation.c
            bluetooth/tinycbor/compilersupport_p.h
            bluetooth/tinycbor/open_memstream.c
            bluetooth/tinycbor/parsetags.pl
            bluetooth/tinycbor/tinycbor-version.h
            bluetooth/tinycbor/utf8_p.h)
target_include_directories(tinycbor PUBLIC bluetooth/tinycbor)

add_executable(sigatewayd
               main.cpp
               sidaemon.h
               sidaemon.cpp
               sisettings.h
               sisettings.cpp
               deviceaccess/sideviceaccessmanager.cpp
               deviceaccess/sideviceaccessmanager.h
               deviceaccess/operations/sipropertyreadoperation.cpp
               deviceaccess/operations/sipropertyreadoperation.h
               deviceaccess/operations/sipropertywriteoperation.cpp
               deviceaccess/operations/sipropertywriteoperation.h
               deviceaccess/operations/siabstractoperation.cpp
               deviceaccess/operations/siabstractoperation.h
               deviceaccess/sisequentialpropertymanager.cpp
               deviceaccess/sisequentialpropertymanager.h
               deviceaccess/operations/sideviceenumerationoperation.cpp
               deviceaccess/operations/sideviceenumerationoperation.h
               deviceaccess/operations/sipropertiesreadoperation.cpp
               deviceaccess/operations/sipropertiesreadoperation.h
               websocket/siwebsocketmanager.cpp
               websocket/siwebsocketmanager.h
               websocket/siwebsocketconnection.cpp
               websocket/siwebsocketconnection.h
               websocket/siwebsocketprotocolframe.cpp
               websocket/siwebsocketprotocolframe.h
               websocket/siabstractwebsocketprotocol.h
               websocket/siwebsocketprotocolv1.h
               websocket/siwebsocketprotocolv1.cpp
               bluetooth/sibluetoothmanager.cpp
               bluetooth/sibluetoothmanager.h
               bluetooth/sibluetoothprotocolframe.cpp
               bluetooth/sibluetoothprotocolframe.h
               bluetooth/siabstractbluetoothprotocol.h
               bluetooth/sibluetoothprotocolv1.h
               bluetooth/sibluetoothprotocolv1.cpp
               datalog/sidatalogconfiguration.h
               datalog/sidatalogconfiguration.cpp
               datalog/sidataloggroup.h
               datalog/sidataloggroup.cpp
               sicontext.h
               datalog/sidatalogmanager.h
               datalog/sidatalogmanager.cpp
               extension/siextensionmanager.cpp
               extension/siextensionmanager.h)
target_link_libraries(sigatewayd Qt5::Core Qt5::Network Qt5::WebSockets Qt5::Bluetooth SIDeviceAccess SIStorage SIAuthorize SIExtension tinycbor)
install(TARGETS sigatewayd RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SBINDIR}")

si_add_library(SIExtension)
si_add_library_public_headers(SIExtension
                              siextensioncontext.h
                              siextensionstatus.h
                              siextensionwebsocketresult.h
                              siextensionbluetoothresult.h
                              siextension.h
                              siextensiondriver.h)
si_add_library_sources(SIExtension
                       siextensionstatus.cpp
                       siextensionwebsocketresult.cpp
                       siextensionbluetoothresult.cpp
                       siextension.cpp
                       siextensiondriver.cpp)
target_link_libraries(SIExtension Qt5::Core SIAuthorize SIDeviceAccess SIStorage)
si_install_library(SIExtension)

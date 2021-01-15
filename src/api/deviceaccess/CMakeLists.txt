si_add_library(SIDeviceAccess)
si_add_library_public_headers(SIDeviceAccess deviceaccess
                              sistatus.h
                              sipropertyid.h
                              sipropertytype.h
                              sipropertyflags.h
                              sipropertyreadresult.h
                              sipropertywriteflags.h
                              sipropertywriteresult.h
                              sidevice.h
                              sijsonflags.h
                              sideviceaccess.h
                              sideviceaccessregistry.h
                              sideviceaccessdriver.h
                              siaccesslevel.h)
si_add_library_sources(SIDeviceAccess
                       sistatus.cpp
                       sipropertytype.cpp
                       sidevice.cpp
                       sideviceaccess.cpp
                       sideviceaccessregistry.cpp
                       siaccesslevel.cpp)
target_include_directories(SIDeviceAccess INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SIDeviceAccess Qt5::Core)
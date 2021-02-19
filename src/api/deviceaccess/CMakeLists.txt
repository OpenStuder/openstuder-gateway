si_add_library(SIDeviceAccess)
si_add_library_public_headers(SIDeviceAccess deviceaccess
                              siaccesslevel.h
                              sidevice.h
                              sideviceaccess.h
                              sideviceaccessdriver.h
                              sideviceaccessregistry.h
                              sidevicemessage.h
                              siglobalpropertyid.h
                              sijdescriptionflags.h
                              siproperty.h
                              sipropertyflags.h
                              sipropertyid.h
                              sipropertyreadresult.h
                              sipropertytype.h
                              sipropertywriteflags.h
                              sipropertywriteresult.h
                              sistatus.h)
si_add_library_sources(SIDeviceAccess
                       siaccesslevel.cpp
                       sidevice.cpp
                       sideviceaccess.cpp
                       sideviceaccessregistry.cpp
                       sidevicemessage.cpp
                       siglobalpropertyid.cpp
                       siproperty.cpp
                       sipropertyreadresult.cpp
                       sipropertytype.cpp
                       sipropertywriteresult.cpp
                       sistatus.cpp)
target_include_directories(SIDeviceAccess INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SIDeviceAccess Qt5::Core)

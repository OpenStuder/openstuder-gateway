si_add_library(SIStorage)
si_add_library_public_headers(SIStorage
                              sistorage.h
                              sipropertyhistorystorage.h
                              sidevicemessagestorage.h
                              sistoragedriver.h)
si_add_library_sources(SIStorage
                       sipropertyhistorystorage.cpp
                       sidevicemessagestorage.cpp
                       sistoragedriver.cpp)
target_link_libraries(SIStorage Qt5::Core SIDeviceAccess)
si_install_library(SIStorage)
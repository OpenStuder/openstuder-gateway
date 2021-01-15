si_add_library(SIStorage)
si_add_library_public_headers(SIStorage storage
                              sistorage.h
                              sipropertyhistorystorage.h
                              sistoragedriver.h)
si_add_library_sources(SIStorage
                       sipropertyhistorystorage.cpp
                       sistoragedriver.cpp)
target_include_directories(SIStorage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SIStorage SIDeviceAccess Qt5::Core)
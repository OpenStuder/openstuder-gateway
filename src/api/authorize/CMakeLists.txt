si_add_library(SIAuthorize)
si_add_library_public_headers(SIAuthorize
                              siuserauthorizer.h
                              siuserauthorizedriver.h
                              sitextfileusermanagement.h)
si_add_library_sources(SIAuthorize
                       siuserauthorizedriver.cpp
                       sitextfileusermanagement.cpp)
target_include_directories(SIAuthorize INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SIAuthorize Qt5::Core SIDeviceAccess)

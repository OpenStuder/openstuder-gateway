si_add_library(SIExtension)
si_add_library_public_headers(SIExtension
                              siextension.h
                              siextensiondriver.h)
si_add_library_sources(SIExtension
                       siextension.cpp
                       siextensiondriver.cpp)
target_link_libraries(SIExtension Qt5::Core)
si_install_library(SIExtension)

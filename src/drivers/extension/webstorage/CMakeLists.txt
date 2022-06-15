find_package(Qt5 5.11 REQUIRED COMPONENTS Sql)
si_add_extension_driver(WebStorage
                        webstorageextension.h
                        webstorageextension.cpp
                        webstorageextensiondriver.h
                        webstorageextensiondriver.cpp)
target_link_libraries(WebStorage Qt5::Sql)

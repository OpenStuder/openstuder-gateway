find_package(Qt5 5.11 REQUIRED COMPONENTS Sql)
si_add_extension_driver(webdata
                      webdataextension.h
                      webdataextension.cpp
                      webdataextensiondriver.h
                      webdataextensiondriver.cpp)
target_link_libraries(webdata Qt5::Sql)

find_package(Qt5 5.11 REQUIRED COMPONENTS Network)
si_add_extension_driver(WifiConfig
                        wificonfigextension.h
                        wificonfigextension.cpp
                        wificonfigextensiondriver.h
                        wificonfigextensiondriver.cpp
                        resources/resources.qrc)
target_link_libraries(WifiConfig Qt5::Network)

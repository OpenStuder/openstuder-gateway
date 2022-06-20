si_add_extension_driver(UserManagement
                        usermanagementextension.h
                        usermanagementextension.cpp
                        usermanagementextensiondriver.h
                        usermanagementextensiondriver.cpp)
target_link_libraries(UserManagement SIDeviceAccess SIAuthorize)

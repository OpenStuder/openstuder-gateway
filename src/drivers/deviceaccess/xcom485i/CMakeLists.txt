find_package(Qt5 5.11 REQUIRED COMPONENTS SerialPort SerialBus)
si_add_device_access_driver(XCom485i
                            xcom485iproperty.h
                            xcom485idevice.h
                            xcom485idevice.cpp
                            xcom485ideviceaccess.h
                            xcom485ideviceaccess.cpp
                            xcom485idriver.h
                            xcom485idriver.cpp
                            xcom485ixtender.h
                            xcom485ixtender.cpp
                            xcom485ivariotrack.h
                            xcom485ivariotrack.cpp
                            xcom485ivariostring.h
                            xcom485ivariostring.cpp
                            xcom485ibatterymanager.h
                            xcom485ibatterymanager.cpp)
target_link_libraries(XCom485i Qt5::SerialPort Qt5::SerialBus)

find_package(Qt5 5.11 REQUIRED COMPONENTS Sql)
si_add_storage_driver(SQLite
                      sqlitestorage.h
                      sqlitestorage.cpp
                      sqlitestoragedriver.h
                      sqlitestoragedriver.cpp)
target_link_libraries(SQLite Qt5::Sql)

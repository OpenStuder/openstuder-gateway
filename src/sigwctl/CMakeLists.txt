add_executable(sigwctl)
target_sources(sigwctl PRIVATE
               siabstractcommand.h
               siabstractcommand.cpp
               sihelpcommand.cpp
               siusermanagementcommand.cpp
               siversioncommand.cpp
               main.cpp)
target_link_libraries(sigwctl Qt5::Core SIAuthorize)
install(TARGETS sigwctl RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")

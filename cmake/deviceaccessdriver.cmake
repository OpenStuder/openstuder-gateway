function(si_add_device_access_driver name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} SIDeviceAccess)
    set_target_properties(${name} PROPERTIES
                          PREFIX ""
                          SUFFIX ".sdd"
                          )
    install(TARGETS ${name} LIBRARY DESTINATION "var/lib/openstuder/drivers")
endfunction()

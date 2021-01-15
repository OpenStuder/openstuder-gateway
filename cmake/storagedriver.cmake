function(si_add_storage_driver NAME)
    add_library(${NAME} MODULE)
    target_sources(${NAME} PRIVATE ${ARGN})
    target_link_libraries(${NAME} SIDeviceAccess SIStorage)
    set_target_properties(${NAME} PROPERTIES
                          PREFIX ""
                          SUFFIX ".ssd"
                          )
    install(TARGETS ${NAME} LIBRARY DESTINATION "var/lib/openstuder/drivers")
endfunction()

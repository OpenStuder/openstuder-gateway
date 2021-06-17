function(si_add_library NAME)
    add_library(${NAME} SHARED)

endfunction()

function(si_add_library_public_headers NAME INSTALL_POSTFIX)
    target_sources(${NAME} PUBLIC ${ARGN})
    set_target_properties(${NAME} PROPERTIES PUBLIC_HEADER "${ARGN}")
endfunction()

function(si_add_library_private_headers NAME)
    target_sources(${NAME} PRIVATE ${ARGN})
endfunction()

function(si_add_library_sources NAME)
    target_sources(${NAME} PRIVATE ${ARGN})
endfunction()

function(si_install_library NAME)
    install(
            TARGETS ${NAME}
            EXPORT ${NAME}_TARGETS
            LIBRARY DESTINATION "${LIBDIR}"
            PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/openstuder/${NAME}"
    )
endfunction()

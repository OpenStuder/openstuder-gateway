function(si_add_library NAME)
    add_library(${NAME} SHARED)
    target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(si_add_library_public_headers NAME)
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
    target_include_directories(${NAME} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/openstuder/${NAME}>
    )
    install(
        TARGETS ${NAME}
        EXPORT ${NAME}_TARGETS
        LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/openstuder/${NAME}"
    )
endfunction()

function(si_add_library NAME)
    add_library(${NAME} SHARED)
    install(TARGETS ${NAME} LIBRARY DESTINATION "lib")
endfunction()

function(si_add_library_public_headers NAME INSTALL_POSTFIX)
    target_sources(${NAME} PUBLIC ${ARGN})
    install(FILES ${ARGN} DESTINATION "include/openstuder/${INSTALL_POSTFIX}")
endfunction()

function(si_add_library_private_headers NAME)
    target_sources(${NAME} PRIVATE ${ARGN})
endfunction()

function(si_add_library_sources NAME)
    target_sources(${NAME} PRIVATE ${ARGN})
endfunction()

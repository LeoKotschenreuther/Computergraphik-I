macro(qt5_auto_wrap outfiles)
    # clear accumulating variables
    unset(moc_headers)
    unset(ui_files)
    unset(qrc_files)

    # generate list of moc, uic, qrc files
    foreach(fileName ${ARGN})
        # moc: headers (WITH .h) need to contain "Q_OBJECT"
        if(fileName MATCHES "\\.h$")
            file(STRINGS ${fileName} lines REGEX Q_OBJECT)
            if(lines)
                set(moc_headers ${moc_headers} ${fileName})
                message(STATUS "moc: ${fileName}")
            endif()
        endif()

        # uic: files have extension ".ui"
        if(fileName MATCHES "\\.ui$")
            set(ui_files ${ui_files} ${fileName})
            message(STATUS "uic: ${fileName}")
        endif()

        # qrc: files have extension ".qrc"
        if(fileName MATCHES "\\.qrc$")
            set(qrc_files ${qrc_files} ${fileName})
            message(STATUS "qrc: ${fileName}")
        endif()
    endforeach()
    
    # use standard functions to handle these files
    qt5_wrap_cpp(${outfiles} ${moc_headers})
    qt5_wrap_ui(${outfiles} ${ui_files})
    qt5_add_resources(${outfiles} ${qrc_files})
    
    # add include directory for generated ui_*.h files
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(qt5_auto_wrap)
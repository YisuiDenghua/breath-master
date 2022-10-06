install(FILES colors metadata.desktop DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath-light/)

FILE(GLOB widgets widgets/*.svgz)
install( FILES ${widgets} DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath-light/widgets/ )

install( CODE "
    execute_process( COMMAND \"${CMAKE_COMMAND}\" -E create_symlink ../breath/icons icons WORKING_DIRECTORY \"\$ENV{DESTDIR}/${CMAKE_INSTALL_PREFIX}/${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath-light\" )
")

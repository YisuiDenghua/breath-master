install(FILES colors metadata.desktop DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath-dark/)

set(ITEMS icons widgets)
foreach(ITEM ${ITEMS})
       install( CODE "
           execute_process( COMMAND \"${CMAKE_COMMAND}\" -E create_symlink ../breath/${ITEM} ${ITEM} WORKING_DIRECTORY \"\$ENV{DESTDIR}/${CMAKE_INSTALL_PREFIX}/${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath-dark\" )
       ")
endforeach(ITEM)

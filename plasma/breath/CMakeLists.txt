install(FILES metadata.desktop DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath/)

FILE(GLOB widgets widgets/*.svgz)
install( FILES ${widgets} DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath/widgets/ )

FILE(GLOB icons icons/*.svgz)
install( FILES ${icons} DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/breath/icons/ )

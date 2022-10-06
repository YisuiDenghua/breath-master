find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5Service ${KF5_MIN_VERSION} REQUIRED)
find_package(Qt5 COMPONENTS DBus REQUIRED)
add_executable(breath2tobreath main.cpp)

target_link_libraries(breath2tobreath KF5::ConfigWidgets KF5::IconThemes KF5::Service Qt5::DBus)

install(TARGETS breath2tobreath DESTINATION ${KDE_INSTALL_LIBDIR}/kconf_update_bin/)
install(FILES breath2tobreath.upd DESTINATION ${KDE_INSTALL_KCONFUPDATEDIR})

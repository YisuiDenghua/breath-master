project(Breath2Theme)

cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

option(BUILD_PLASMA_THEMES "Build Plasma Themes")
option(BUILD_SDDM_THEME "Build SDDM Theme")
option(BUILD_EXTRA_COLORS "Build extra KDE colors")
option(BUILD_MIGRATION "Build kconf_update-based user config migration routine")

include(WriteBasicConfigVersionFile)
include(FeatureSummary)

find_package(ECM 1.5.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

# Set version
set(THEME_VERSION_MAJOR 21)
set(THEME_VERSION_MINOR 2)
set(THEME_VERSION_PATCH 0)
set(THEME_VERSION_STRING "${THEME_VERSION_MAJOR}.${THEME_VERSION_MINOR}.${THEME_VERSION_PATCH}")

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)

include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(GenerateExportHeader)

if(BUILD_PLASMA_THEMES)
    message(STATUS "build plasma-themes")
    add_subdirectory(lnf)
    add_subdirectory(plasma)
    add_subdirectory(wallpapers)
    add_subdirectory(colors)
    add_subdirectory(layout-templates)
    add_subdirectory(konsole)
    add_subdirectory(yakuake)
endif(BUILD_PLASMA_THEMES)

if(BUILD_EXTRA_COLORS)
    message(STATUS "build extra colors")
    add_subdirectory(colors/extra)
endif(BUILD_EXTRA_COLORS)

if(BUILD_MIGRATION)
    message(STATUS "build kconf_update-based migration routine")
    add_subdirectory(migration)
endif(BUILD_MIGRATION)

if(BUILD_SDDM_THEME)
    message(STATUS "build SDDM theme")
    configure_file(sddm-theme/theme.conf.cmake ${CMAKE_CURRENT_BINARY_DIR}/sddm-theme/theme.conf)

    install(DIRECTORY sddm-theme/ DESTINATION ${KDE_INSTALL_FULL_DATADIR}/sddm/themes/breath
            PATTERN "README.txt" EXCLUDE
            PATTERN "dummydata" EXCLUDE
            PATTERN "theme.conf.cmake" EXCLUDE)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sddm-theme/theme.conf DESTINATION ${KDE_INSTALL_FULL_DATADIR}/sddm/themes/breath)

endif(BUILD_SDDM_THEME)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

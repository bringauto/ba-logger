CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(libbringauto_logger)

SET(CMAKE_CXX_STANDARD 17)

SET(libbringauto_logger_VERSION 1.1.0)
SET(LIB_TYPE "SPDLOG" CACHE STRING "Set implementation that will be compiled")

OPTION(BRINGAUTO_SAMPLES OFF)
OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package generation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Enable system dependencies" OFF)

IF(NOT BRINGAUTO_SYSTEM_DEP)
    FIND_PACKAGE(CMLIB REQUIRED)
    INCLUDE(cmake/Dependencies.cmake)
ENDIF()

FUNCTION(LOGGER_CHECK_SYMBOLS)
    INCLUDE(CheckSymbolExists)
    CHECK_SYMBOL_EXISTS(stat "sys/stat.h" HAVE_STAT)
    CHECK_SYMBOL_EXISTS(getuid "unistd.h" HAVE_GETUID)
    CHECK_SYMBOL_EXISTS(getgroups "unistd.h" HAVE_GETGROUPS)
    IF(NOT (HAVE_STAT AND HAVE_GETUID AND HAVE_GETGROUPS))
        MESSAGE(FATAL_ERROR "required functions not found")
    ENDIF()
ENDFUNCTION()

LOGGER_CHECK_SYMBOLS()



SET(_SUPPORTED_LIB_TYPES "DUMMY" "SPDLOG")
IF (NOT "${LIB_TYPE}" IN_LIST _SUPPORTED_LIB_TYPES)
    MESSAGE(FATAL_ERROR "Library type ${LIB_TYPE} is not supported.")
ENDIF ()
SET(LIB_TYPES_TO_BUILD "${LIB_TYPE}")

IF (BRINGAUTO_ALL_TESTS)
    LIST(APPEND LIB_TYPES_TO_BUILD ${_SUPPORTED_LIB_TYPES})
    LIST(REMOVE_DUPLICATES LIB_TYPES_TO_BUILD)
ELSEIF (BRINGAUTO_TESTS)
    LIST(APPEND LIB_TYPES_TO_BUILD "DUMMY")
    LIST(REMOVE_DUPLICATES LIB_TYPES_TO_BUILD)
ENDIF (BRINGAUTO_ALL_TESTS)

ADD_LIBRARY(bringauto_logger_api INTERFACE)
SET_TARGET_PROPERTIES(bringauto_logger_api PROPERTIES LINKER_LANGUAGE CXX)
TARGET_INCLUDE_DIRECTORIES(bringauto_logger_api INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/libbringauto_logger>
        )

FOREACH (_TO_BUILD IN LISTS LIB_TYPES_TO_BUILD)
    STRING(TOLOWER ${_TO_BUILD} _TO_BUILD_NAME)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/log_lib/${_TO_BUILD_NAME}/)
ENDFOREACH ()

IF (${BRINGAUTO_SAMPLES})
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/example/)
ENDIF ()

IF (BRINGAUTO_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tests/)
ENDIF ()


FUNCTION(BRINGAUTO_LOGGER_INSTALL)
    STRING(TOLOWER ${LIB_TYPE} _LIB_TYPE_TO_INSTALL)

    ADD_LIBRARY(bringauto_logger INTERFACE)
    TARGET_LINK_LIBRARIES(bringauto_logger INTERFACE bringauto_logger_api bringauto_logger_${_LIB_TYPE_TO_INSTALL})

    INCLUDE(GNUInstallDirs)

    INSTALL(TARGETS bringauto_logger_${_LIB_TYPE_TO_INSTALL} EXPORT libbringauto_loggerTargets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include
            )

    INSTALL(TARGETS bringauto_logger_api EXPORT libbringauto_loggerTargets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include
            )

    INSTALL(TARGETS bringauto_logger EXPORT libbringauto_loggerTargets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include
            )

    INSTALL(
            DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/include/bringauto
            DESTINATION
            include/libbringauto_logger
            COMPONENT
            Devel
    )

    INSTALL(
            DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/include/fmt
            DESTINATION
            include/libbringauto_logger
            COMPONENT
            Devel
    )

    INCLUDE(CMakePackageConfigHelpers)
    CONFIGURE_PACKAGE_CONFIG_FILE(
        cmake/libbringauto_loggerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/libbringauto_logger/libbringauto_loggerConfig.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/libbringauto_logger/cmake
    )

    WRITE_BASIC_PACKAGE_VERSION_FILE(
        "${CMAKE_CURRENT_BINARY_DIR}/libbringauto_logger/libbringauto_loggerConfigVersion.cmake"
        VERSION ${libbringauto_logger_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    EXPORT(EXPORT libbringauto_loggerTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/libbringauto_logger/libbringauto_loggerTargets.cmake"
        )

    SET(ConfigPackageLocation lib/cmake/libbringauto_logger)
    INSTALL(EXPORT libbringauto_loggerTargets
        FILE
        libbringauto_loggerTargets.cmake
        NAMESPACE
        bringauto_logger::
        DESTINATION
        ${ConfigPackageLocation}
        )

    INSTALL(
        FILES
        cmake/libbringauto_loggerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/libbringauto_logger/libbringauto_loggerConfigVersion.cmake
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
    )
ENDFUNCTION()

FUNCTION(bringauto_logger_package)
    SET(CPACK_GENERATOR ZIP)
    SET(CPACK_PACKAGE_CONTACT "Jakub Trubka <jakub.trubka@bringauto.com>")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jakub Trubka <jakub.trubka@bringauto.com>")
    SET(CPACK_PACKAGE_VERSION ${libbringauto_logger_VERSION})
    INCLUDE(CPack)
ENDFUNCTION()

IF (BRINGAUTO_INSTALL)
    BRINGAUTO_LOGGER_INSTALL()
ENDIF()

IF (BRINGAUTO_PACKAGE)
    IF(NOT BRINGAUTO_INSTALL)
        SET(BRINGAUTO_INSTALL ON CACHE BOOL "Forced install due to BRINGAUTO_PACKAGE=ON" FORCE)
        MESSAGE(WARNING "BRINGAUTO_INSTALL is switched to on because of BRINGAUTO_PACKAGE=ON")
        BRINGAUTO_LOGGER_INSTALL()
    ENDIF()
    BRINGAUTO_LOGGER_PACKAGE()
ENDIF()



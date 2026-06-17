INCLUDE_GUARD(GLOBAL)
# Provides: spdlog::spdlog
# Resolution controlled by BA_PACKAGE_SOURCE:
#   AUTO             (default) system package → prebuilt zip → FetchContent, stops at first success
#   SYSTEM_PACKAGES  system-installed spdlog only; fatal error if not found
#   PREBUILT_PACKAGES prebuilt zip from Gitea only; fatal error if not found
#   FETCH_CONTENT    clone and build from source via FetchContent
IF(NOT BA_PACKAGE_SOURCE MATCHES "^(AUTO|SYSTEM_PACKAGES|PREBUILT_PACKAGES|FETCH_CONTENT)$")
    MESSAGE(FATAL_ERROR "[BA] Invalid BA_PACKAGE_SOURCE='${BA_PACKAGE_SOURCE}'. Must be one of: AUTO, SYSTEM_PACKAGES, PREBUILT_PACKAGES, FETCH_CONTENT")
ENDIF()

IF(spdlog_DIR MATCHES "${CMAKE_BINARY_DIR}")
    UNSET(spdlog_DIR CACHE)
ENDIF()

IF(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "SYSTEM_PACKAGES")
    FIND_PACKAGE(spdlog 1.14.1 QUIET CONFIG)
    IF(spdlog_FOUND)
        MESSAGE(STATUS "[BA] spdlog: found via system package")
        SET(BASpdlog_FOUND TRUE)
        RETURN()
    ENDIF()
    IF(BA_PACKAGE_SOURCE STREQUAL "SYSTEM_PACKAGES")
        MESSAGE(FATAL_ERROR "[BA] spdlog: system package not found (BA_PACKAGE_SOURCE=SYSTEM_PACKAGES)")
    ENDIF()
ENDIF()

IF(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
    INCLUDE(BAFetchDependency OPTIONAL RESULT_VARIABLE _ba_fetchdep_found)
    IF(NOT _ba_fetchdep_found AND BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
        MESSAGE(FATAL_ERROR "[BA] spdlog: BAFetchDependency module not found (required for BA_PACKAGE_SOURCE=PREBUILT_PACKAGES)")
    ENDIF()
    IF(_ba_fetchdep_found)
        BA_PREBUILT_PACKAGE(spdlog 1.14.1 _ba_spdlog_prebuilt)
    ENDIF()
    FIND_PACKAGE(spdlog 1.14.1 QUIET CONFIG)
    IF(spdlog_FOUND)
        MESSAGE(STATUS "[BA] spdlog: found via prebuilt package")
        SET(BASpdlog_FOUND TRUE)
        RETURN()
    ENDIF()
    IF(BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
        MESSAGE(FATAL_ERROR "[BA] spdlog: prebuilt package not found (BA_PACKAGE_SOURCE=PREBUILT_PACKAGES)")
    ENDIF()
ENDIF()

IF(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "FETCH_CONTENT")
    MESSAGE(STATUS "[BA] spdlog: fetching via FetchContent")
    INCLUDE(FetchContent)
    FETCHCONTENT_DECLARE(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        27cb4c76708608465c413f6d0e6b8d99a4d84302)
    FETCHCONTENT_MAKEAVAILABLE(spdlog)
    SET(BASpdlog_FOUND TRUE)
ENDIF()

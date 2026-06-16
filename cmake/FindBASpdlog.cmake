include_guard(GLOBAL)
# Provides: spdlog::spdlog
# Resolution controlled by BA_PACKAGE_SOURCE:
#   AUTO             (default) system package → prebuilt zip → FetchContent, stops at first success
#   SYSTEM_PACKAGES  system-installed spdlog only; fatal error if not found
#   PREBUILT_PACKAGES prebuilt zip from Gitea only; fatal error if not found
#   FETCH_CONTENT    clone and build from source via FetchContent
if(NOT BA_PACKAGE_SOURCE MATCHES "^(AUTO|SYSTEM_PACKAGES|PREBUILT_PACKAGES|FETCH_CONTENT)$")
    message(FATAL_ERROR "[BA] Invalid BA_PACKAGE_SOURCE='${BA_PACKAGE_SOURCE}'. Must be one of: AUTO, SYSTEM_PACKAGES, PREBUILT_PACKAGES, FETCH_CONTENT")
endif()

if(spdlog_DIR MATCHES "${CMAKE_BINARY_DIR}")
    unset(spdlog_DIR CACHE)
endif()

if(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "SYSTEM_PACKAGES")
    find_package(spdlog 1.14.1 QUIET CONFIG)
    if(spdlog_FOUND)
        message(STATUS "[BA] spdlog: found via system package")
        set(BASpdlog_FOUND TRUE)
        return()
    endif()
    if(BA_PACKAGE_SOURCE STREQUAL "SYSTEM_PACKAGES")
        message(FATAL_ERROR "[BA] spdlog: system package not found (BA_PACKAGE_SOURCE=SYSTEM_PACKAGES)")
    endif()
endif()

if(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
    include(BAFetchDependency OPTIONAL RESULT_VARIABLE _ba_fetchdep_found)
    if(NOT _ba_fetchdep_found AND BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
        message(FATAL_ERROR "[BA] spdlog: BAFetchDependency module not found (required for BA_PACKAGE_SOURCE=PREBUILT_PACKAGES)")
    endif()
    if(_ba_fetchdep_found)
        ba_prebuilt_package(spdlog 1.14.1 _ba_spdlog_prebuilt)
    endif()
    find_package(spdlog 1.14.1 QUIET CONFIG)
    if(spdlog_FOUND)
        message(STATUS "[BA] spdlog: found via prebuilt package")
        set(BASpdlog_FOUND TRUE)
        return()
    endif()
    if(BA_PACKAGE_SOURCE STREQUAL "PREBUILT_PACKAGES")
        message(FATAL_ERROR "[BA] spdlog: prebuilt package not found (BA_PACKAGE_SOURCE=PREBUILT_PACKAGES)")
    endif()
endif()

if(BA_PACKAGE_SOURCE STREQUAL "AUTO" OR BA_PACKAGE_SOURCE STREQUAL "FETCH_CONTENT")
    message(STATUS "[BA] spdlog: fetching via FetchContent")
    include(FetchContent)
    FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.14.1
        GIT_SHALLOW    TRUE)
    FetchContent_MakeAvailable(spdlog)
    set(BASpdlog_FOUND TRUE)
endif()

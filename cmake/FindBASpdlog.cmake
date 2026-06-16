include_guard(GLOBAL)
# Provides: spdlog::spdlog
# Resolution order: system package → prebuilt zip → FetchContent source build.
# System spdlog is preferred: the prebuilt ships without -fPIC and may bundle
# a fmtlib version that conflicts with existing code.
# BAFetchDependency is optional so this script remains usable without it
# (e.g. when loaded from an installed fhr_cpp_utils package config).
# Flush stale FetchContent-internal dir; preserve explicit user hint.
if(spdlog_DIR MATCHES "${CMAKE_BINARY_DIR}")
    unset(spdlog_DIR CACHE)
endif()
find_package(spdlog 1.14.1 QUIET CONFIG)
if(spdlog_FOUND)
    message(STATUS "[BA] spdlog: found via system package")
    set(BASpdlog_FOUND TRUE)
    return()
endif()
include(BAFetchDependency OPTIONAL RESULT_VARIABLE _ba_fetchdep_found)
if(_ba_fetchdep_found)
    ba_prebuilt_package(spdlog 1.14.1 _ba_spdlog_prebuilt)
endif()
find_package(spdlog 1.14.1 QUIET CONFIG)
if(spdlog_FOUND)
    message(STATUS "[BA] spdlog: found via prebuilt package")
    set(BASpdlog_FOUND TRUE)
    return()
endif()
message(STATUS "[BA] spdlog: prebuilt/system not found, fetching via FetchContent")
include(FetchContent)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(spdlog)
set(BASpdlog_FOUND TRUE)

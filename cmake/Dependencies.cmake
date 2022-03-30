CMLIB_DEPENDENCY(
        URI "https://github.com/bringauto/spdlog-package.git"
        URI_TYPE GIT
        GIT_TAG v1.8.5
        TYPE MODULE
)
FIND_PACKAGE(spdlog_package REQUIRED)

FILE(GLOB_RECURSE spdlog_source_files "${CMAKE_CURRENT_LIST_DIR}/src/*")
ADD_LIBRARY(bringauto_logger_spdlog SHARED ${spdlog_source_files})

INCLUDE(GenerateExportHeader)
GENERATE_EXPORT_HEADER(bringauto_logger_spdlog)

TARGET_INCLUDE_DIRECTORIES(bringauto_logger_spdlog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include/libbringauto_logger>
)

IF(NOT TARGET spdlog)
    FIND_PACKAGE(spdlog 1.14.1 REQUIRED)
ENDIF ()

IF(NOT TARGET bringauto_logger_api)
    MESSAGE(FATAL_ERROR "bringauto_logger_api target was not found")
ENDIF()

TARGET_LINK_LIBRARIES(bringauto_logger_spdlog PUBLIC bringauto_logger_api)
TARGET_LINK_LIBRARIES(bringauto_logger_spdlog PRIVATE spdlog::spdlog_header_only)

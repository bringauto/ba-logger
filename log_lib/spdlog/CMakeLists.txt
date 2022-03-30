FILE(GLOB_RECURSE spdlog_source_files "${CMAKE_CURRENT_LIST_DIR}/src/*")
ADD_LIBRARY(bringauto_logger_spdlog SHARED ${spdlog_source_files})

include(GenerateExportHeader)
generate_export_header(bringauto_logger_spdlog)

target_include_directories(bringauto_logger_spdlog PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include/libbringauto_logger>
)

IF (NOT TARGET spdlog)
    FIND_PACKAGE(spdlog 1.8.5 REQUIRED)
ENDIF ()

if (NOT TARGET bringauto_logger_api)
    MESSAGE(FATAL_ERROR "bringauto_logger_api target was not found")
endif()

TARGET_LINK_LIBRARIES(bringauto_logger_spdlog PUBLIC bringauto_logger_api)
TARGET_LINK_LIBRARIES(bringauto_logger_spdlog PRIVATE spdlog::spdlog_header_only)



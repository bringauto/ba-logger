
if (NOT TARGET bringauto_logger_api)
    MESSAGE(FATAL_ERROR "bringauto_logger_api target was not found")
endif()

FILE(GLOB_RECURSE dummy_source_files "${CMAKE_CURRENT_LIST_DIR}/src/*")
ADD_LIBRARY(bringauto_logger_dummy SHARED ${dummy_source_files})
TARGET_LINK_LIBRARIES(bringauto_logger_dummy PUBLIC bringauto_logger_api)
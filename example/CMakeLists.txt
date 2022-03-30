ADD_EXECUTABLE(bringauto_logger_sample_app ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

IF("${LIB_TYPE}" STREQUAL "")
    MESSAGE(FATAL_ERROR "LIB_TYPE variable was not setup")
ENDIF()

STRING(TOLOWER ${LIB_TYPE} _LIB_TYPE_NAME)

if (NOT TARGET bringauto_logger_${_LIB_TYPE_NAME})
    MESSAGE(FATAL_ERROR "bringauto_logger_${_LIB_TYPE_NAME} target was not found")
endif()

TARGET_LINK_LIBRARIES(bringauto_logger_sample_app PUBLIC bringauto_logger_${_LIB_TYPE_NAME})
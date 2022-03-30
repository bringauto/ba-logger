FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

if (NOT TARGET bringauto_logger_dummy)
    MESSAGE(FATAL_ERROR "bringauto_logger_dummy target was not found")
endif()


FILE(GLOB_RECURSE source_api_test_files "${CMAKE_CURRENT_LIST_DIR}/src/apiTesting/*")

ADD_EXECUTABLE(liblogger_api_tests ${source_api_test_files} ${CMAKE_CURRENT_LIST_DIR}/mainTests.cpp)
TARGET_INCLUDE_DIRECTORIES(liblogger_api_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/apiTesting")
TARGET_LINK_LIBRARIES(liblogger_api_tests PUBLIC ${GTEST_LIBRARIES} pthread bringauto_logger_dummy)

INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(liblogger_api_tests)

IF("${LIB_TYPE}" STREQUAL "")
    MESSAGE(FATAL_ERROR "LIB_TYPE variable was not setup")
ENDIF()

FILE(GLOB_RECURSE source_implementation_test_files "${CMAKE_CURRENT_LIST_DIR}/src/implementationTesting/*")
FOREACH(LIB_TYPE IN LISTS LIB_TYPES_TO_BUILD)
    IF(NOT "${LIB_TYPE}" STREQUAL "DUMMY")

        STRING(TOLOWER ${LIB_TYPE} _LIB_TYPE_NAME)
        if (NOT TARGET bringauto_logger_${_LIB_TYPE_NAME})
            MESSAGE(FATAL_ERROR "bringauto_logger_${_LIB_TYPE_NAME} target was not found")
        endif()

        ADD_EXECUTABLE(liblogger_implementation_${_LIB_TYPE_NAME}_tests ${source_implementation_test_files} ${CMAKE_CURRENT_LIST_DIR}/mainTests.cpp)
        TARGET_INCLUDE_DIRECTORIES(liblogger_implementation_${_LIB_TYPE_NAME}_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/implementationTesting")
        TARGET_LINK_LIBRARIES(liblogger_implementation_${_LIB_TYPE_NAME}_tests PUBLIC ${GTEST_LIBRARIES} pthread bringauto_logger_${_LIB_TYPE_NAME})

        GTEST_DISCOVER_TESTS(liblogger_implementation_${_LIB_TYPE_NAME}_tests)
    ENDIF()
ENDFOREACH()



target_sources(sup-log
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/basic_logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/default_loggers.cpp
        ${CMAKE_CURRENT_LIST_DIR}/log_severity.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

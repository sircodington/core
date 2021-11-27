project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Logger.h
    Logging.h
)

set(${PROJECT_NAME}_SOURCES
    Logging.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

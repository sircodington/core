project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Logger.h
    Types.h
    Logging.h
    StringView.h
    Either.h
)

set(${PROJECT_NAME}_SOURCES
    Logging.cpp
    StringView.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

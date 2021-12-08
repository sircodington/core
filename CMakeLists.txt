project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Either.h
    Formatter.h
    Formatting.h
    List.h
    String.h
    StringBuilder.h
    StringView.h
    Types.h
)

set(${PROJECT_NAME}_SOURCES
    Formatter.cpp
    String.cpp
    StringBuilder.cpp
    StringView.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

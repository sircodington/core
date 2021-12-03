project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Formatter.h
    Types.h
        Formatting.h
    String.h
    StringBuilder.h
    StringView.h
    Either.h
    List.h
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

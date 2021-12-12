project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Arena.h
    Either.h
    Formatter.h
    Formatting.h
    List.h
    Map.h
    Set.h
    String.h
    StringBuilder.h
    StringView.h
    Types.h
)

set(${PROJECT_NAME}_SOURCES
    Arena.cpp
    Formatter.cpp
    String.cpp
    StringBuilder.cpp
    StringView.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

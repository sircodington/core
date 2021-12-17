project(core LANGUAGES C CXX)

set(${PROJECT_NAME}_HEADERS
    Arena.h
    BitField.h
    Either.h
    Formatter.h
    Formatting.h
    Files.h
    List.h
    ListView.h
    Map.h
    Set.h
    StopWatch.h
    String.h
    StringBuilder.h
    StringView.h
    Types.h
)

set(${PROJECT_NAME}_SOURCES
    Arena.cpp
    Formatter.cpp
    Files.cpp
    StopWatch.cpp
    String.cpp
    StringBuilder.cpp
    StringView.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

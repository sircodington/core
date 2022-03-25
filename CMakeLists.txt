cmake_minimum_required(VERSION 3.20)
project(core LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wno-literal-suffix)
add_compile_options(-Wswitch)

set(${PROJECT_NAME}_HEADERS
    include/core/Arena.h
    include/core/BitField.h
    include/core/Either.h
    include/core/Formatter.h
    include/core/Formatting.h
    include/core/Files.h
    include/core/IterationDecision.h
    include/core/LexicalFile.h
    include/core/List.h
    include/core/ListView.h
    include/core/Map.h
    include/core/RingBuffer.h
    include/core/Set.h
    include/core/StopWatch.h
    include/core/String.h
    include/core/StringBuilder.h
    include/core/StringView.h
    include/core/Types.h
)

set(${PROJECT_NAME}_SOURCES
    src/Arena.cpp
    src/Formatter.cpp
    src/LexicalFile.cpp
    src/Files.cpp
    src/StopWatch.cpp
    src/String.cpp
    src/StringBuilder.cpp
    src/StringView.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_TEST_SOURCES
    test/test_lexical_file.cpp
    test/test_string_view.cpp
    test/main.cpp
)
add_executable(${PROJECT_NAME}-test
    ${${PROJECT_NAME}_TEST_SOURCES}
)
target_include_directories(${PROJECT_NAME}-test
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}-test
    core
)

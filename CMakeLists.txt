project(dwarf-crawler)
cmake_minimum_required(VERSION 2.8)

set(SOURCES
    main.cpp
    IDwarfCrawler.h
    DwarfCrawler.cpp
    Config.h
    Config.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} libdwarf.a libelf.a -lz)

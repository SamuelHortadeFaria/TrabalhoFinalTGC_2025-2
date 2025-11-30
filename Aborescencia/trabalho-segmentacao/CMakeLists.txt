cmake_minimum_required(VERSION 3.10)

project(SegmentacaoImagem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Arborescence.cpp
    GraphBuilder.cpp
    Image.cpp
    main.cpp
    MST.cpp
    Segmenter.cpp
    utils.cpp
)

set(HEADERS
    Arborescence.h
    Edge.h
    GraphBuilder.h
    Image.h
    MST.h
    Segmenter.h
    stb_image.h
    stb_image_write.h
    utils.h
)

add_executable(segmentacao ${SOURCES} ${HEADERS})

target_include_directories(segmentacao PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    target_link_libraries(segmentacao m)
endif()
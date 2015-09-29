cmake_minimum_required (VERSION 2.6)
project (testcam)

find_package (OpenCV REQUIRED)

set (SOURCES
    main.cpp
    serialib.cpp
    )

add_executable (testcam ${SOURCES})
target_link_libraries (testcam ${OpenCV_LIBS})

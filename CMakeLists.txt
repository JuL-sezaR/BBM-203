cmake_minimum_required(VERSION 3.10)

project(HUTrain VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Add source files
set(SOURCES
    main.cpp
    Wagon.cpp
    WagonList.cpp
    Train.cpp
    TrainTrack.cpp
    ClassificationYard.cpp
    RailMarshal.cpp
    Enums.cpp
)

set(HEADERS
    Wagon.h
    WagonList.h
    Train.h
    TrainTrack.h
    ClassificationYard.h
    RailMarshal.h
    Enums.h
)

# Add the executable
add_executable(HUTrain ${SOURCES} ${HEADERS})

# Include directories (for headers)
target_include_directories(HUTrain PRIVATE ${CMAKE_SOURCE_DIR})

# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)
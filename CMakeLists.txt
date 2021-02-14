cmake_minimum_required(VERSION 3.5)

message("Googletest is cloned from repository...")
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

# Устанавливаем стандарты для компиляции библиотеки googletest
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES CXX_STANDARD 11)
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES CXX_STANDARD_REQUIRED ON)
message("Googletest is cloned")

# Now simply link your own targets against gtest, gmock,
# etc. as appropriate

message("Project GUI building is started ...")
project(gui LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)

set(PROJECT_SOURCES
        gui/main.cpp
        gui/mainwindow.cpp
        gui/mainwindow.h
        gui/qcustomplot.cpp
        gui/qcustomplot.h
        gui/functions.cpp
        gui/functions.h
        spline/spline.cpp
        spline/spline.h
        lagrange/lagrange.cpp
        lagrange/lagrange.h
)

add_executable(gui ${PROJECT_SOURCES})

set_target_properties(gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./bin")
target_link_libraries(gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
message("Project GUI building is finished")

message("Project GTests building is started...")
project(GTests LANGUAGES CXX)
add_executable(GTests test/test.cpp spline/spline.cpp)
set_target_properties(GTests PROPERTIES CXX_STANDARD 11)
set_target_properties(GTests PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(GTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./bin")
target_link_libraries(GTests gtest gtest_main)
message("Project GTests building is finished")

install(TARGETS gui RUNTIME DESTINATION bin)
install(TARGETS GTests RUNTIME DESTINATION bin)